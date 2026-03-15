"""Interact with Ghidra MCP via curl. Proper MCP initialization handshake."""
import subprocess, json, sys, time, os, tempfile


HOST = "localhost"
PORT = 9998
BASE = f"http://{HOST}:{PORT}"
SSE_FILE = os.path.join(tempfile.gettempdir(), "ghidra_sse.txt")


def start_sse():
    """Start curl SSE in background, return (process, session_endpoint)."""
    try:
        os.remove(SSE_FILE)
    except FileNotFoundError:
        pass

    proc = subprocess.Popen(
        ["curl", "-s", "-N", f"{BASE}/sse"],
        stdout=open(SSE_FILE, "w"),
        stderr=subprocess.DEVNULL,
    )

    deadline = time.time() + 10
    while time.time() < deadline:
        time.sleep(0.2)
        try:
            with open(SSE_FILE, "r") as f:
                content = f.read()
            for line in content.split("\n"):
                if line.startswith("data:") and "sessionId=" in line:
                    return proc, line[5:].strip()
        except FileNotFoundError:
            pass
    proc.kill()
    raise Exception("Failed to get SSE session")


def read_sse_all():
    """Read all messages from SSE file."""
    try:
        with open(SSE_FILE, "r") as f:
            content = f.read()
    except FileNotFoundError:
        return []

    results = []
    for line in content.split("\n"):
        line = line.strip()
        if line.startswith("data:") and not "sessionId=" in line:
            data = line[5:].strip()
            if data:
                try:
                    results.append(json.loads(data))
                except json.JSONDecodeError:
                    pass
    return results


def post_fire(endpoint, payload):
    """POST JSON-RPC. Returns the subprocess (fire-and-forget)."""
    url = f"{BASE}{endpoint}"
    return subprocess.Popen(
        ["curl", "-s", "-X", "POST", url,
         "-H", "Content-Type: application/json",
         "-d", json.dumps(payload),
         "--max-time", "120"],
        stdout=subprocess.PIPE,
        stderr=subprocess.DEVNULL,
    )


def post_and_wait(endpoint, payload, timeout=30):
    """POST JSON-RPC and capture the response from stdout (POST body)."""
    url = f"{BASE}{endpoint}"
    try:
        result = subprocess.run(
            ["curl", "-s", "-X", "POST", url,
             "-H", "Content-Type: application/json",
             "-d", json.dumps(payload),
             "--max-time", str(timeout)],
            capture_output=True, text=True, timeout=timeout + 5
        )
        if result.stdout.strip():
            return json.loads(result.stdout)
    except (subprocess.TimeoutExpired, json.JSONDecodeError) as e:
        print(f"POST error: {e}", file=sys.stderr)
    return None


def wait_for_sse_response(req_id, timeout=30):
    """Wait for response on SSE stream."""
    deadline = time.time() + timeout
    while time.time() < deadline:
        time.sleep(0.5)
        for msg in read_sse_all():
            if msg.get("id") == req_id and ("result" in msg or "error" in msg):
                return msg
    return None


def respond_to_pings(endpoint):
    """Check for and respond to server pings."""
    responded = set()
    for msg in read_sse_all():
        if "method" in msg and msg.get("method") == "ping" and "id" in msg:
            pid = msg["id"]
            if pid not in responded:
                responded.add(pid)
                pong = {"jsonrpc": "2.0", "id": pid, "result": {}}
                print(f"Responding to ping {pid}", file=sys.stderr)
                post_fire(endpoint, pong)
    return len(responded)


def extract_text(result):
    if not result:
        return "No response received"
    if "error" in result:
        return f"Error: {json.dumps(result['error'])}"
    r = result.get("result", {})
    content = r.get("content", [])
    texts = [item["text"] for item in content if item.get("type") == "text"]
    return "\n".join(texts) if texts else json.dumps(result, indent=2)


def main():
    action = sys.argv[1] if len(sys.argv) > 1 else "list_tools"

    sse_proc, endpoint = start_sse()
    print(f"Session: {endpoint}", file=sys.stderr)

    # Handle initial pings
    time.sleep(1)
    respond_to_pings(endpoint)
    time.sleep(1)

    # MCP initialization handshake
    print("Sending initialize...", file=sys.stderr)
    init_payload = {
        "jsonrpc": "2.0", "id": "init",
        "method": "initialize",
        "params": {
            "protocolVersion": "2024-11-05",
            "capabilities": {},
            "clientInfo": {"name": "ghidra-cli", "version": "1.0.0"}
        }
    }

    # Try: check if response comes in POST body
    init_proc = post_fire(endpoint, init_payload)

    # Check both: SSE and POST stdout
    init_result = None
    deadline = time.time() + 15
    while time.time() < deadline and init_result is None:
        time.sleep(0.5)
        # Check SSE
        init_result = wait_for_sse_response("init", timeout=0)
        if init_result:
            break
        # Check POST stdout (non-blocking)
        if init_proc.poll() is not None:
            stdout = init_proc.stdout.read().decode() if init_proc.stdout else ""
            if stdout.strip():
                try:
                    init_result = json.loads(stdout)
                except json.JSONDecodeError:
                    pass
            break
        # Handle any new pings
        respond_to_pings(endpoint)

    if init_result:
        print(f"Initialize response: {json.dumps(init_result)[:200]}", file=sys.stderr)
    else:
        print("No initialize response received", file=sys.stderr)
        # Dump SSE file for debugging
        print("SSE file contents:", file=sys.stderr)
        try:
            with open(SSE_FILE, "r") as f:
                print(f.read()[:1000], file=sys.stderr)
        except:
            pass

    # Send initialized notification
    notif = {
        "jsonrpc": "2.0",
        "method": "notifications/initialized",
        "params": {}
    }
    post_fire(endpoint, notif)
    time.sleep(1)
    respond_to_pings(endpoint)
    time.sleep(1)

    try:
        if action == "list_tools":
            payload = {"jsonrpc": "2.0", "id": "list1", "method": "tools/list", "params": {}}
            post_fire(endpoint, payload)
            result = wait_for_sse_response("list1", timeout=15)
            if not result:
                # Also check POST response (maybe MCP server version uses POST body)
                pass
            if result and "result" in result and "tools" in result["result"]:
                for tool in result["result"]["tools"]:
                    desc = tool.get("description", "")[:120].replace("\n", " ")
                    schema = tool.get("inputSchema", {}).get("properties", {})
                    params = list(schema.keys())
                    print(f"  {tool['name']}({', '.join(params)}): {desc}")
            else:
                print(json.dumps(result, indent=2) if result else "No response")

        elif action == "call":
            tool_name = sys.argv[2]
            args = json.loads(sys.argv[3]) if len(sys.argv) > 3 else {}
            timeout = int(sys.argv[4]) if len(sys.argv) > 4 else 30
            payload = {
                "jsonrpc": "2.0", "id": "call1",
                "method": "tools/call",
                "params": {"name": tool_name, "arguments": args}
            }
            post_fire(endpoint, payload)
            result = wait_for_sse_response("call1", timeout=timeout)
            print(extract_text(result))
    finally:
        sse_proc.kill()


if __name__ == "__main__":
    main()
