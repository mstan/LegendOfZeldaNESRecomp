"""Debug raw SSE data from Ghidra MCP."""
import socket, json, urllib.request, time, sys

HOST, PORT = "localhost", 9998

sock = socket.create_connection((HOST, PORT))
sock.settimeout(30)
req = "GET /sse HTTP/1.1\r\nHost: localhost:9998\r\nAccept: text/event-stream\r\nConnection: keep-alive\r\n\r\n"
sock.sendall(req.encode())

# Read initial response + endpoint
buf = b""
endpoint = None
while not endpoint:
    chunk = sock.recv(4096)
    if not chunk:
        break
    buf += chunk
    text = buf.decode("utf-8", errors="replace")
    for line in text.split("\n"):
        line = line.strip()
        if line.startswith("data:") and "sessionId=" in line:
            endpoint = line[5:].strip()
            break

print(f"Endpoint: {endpoint}")
print(f"Initial buffer ({len(buf)} bytes):")
print(repr(buf[:500]))
print()

# POST tools/list
url = f"http://{HOST}:{PORT}{endpoint}"
payload = json.dumps({"jsonrpc": "2.0", "id": 1, "method": "tools/list", "params": {}}).encode()
r = urllib.request.Request(url, data=payload, headers={"Content-Type": "application/json"})
urllib.request.urlopen(r)
print("POST sent")

# Read response
sock.settimeout(3)
raw = b""
for _ in range(10):
    try:
        chunk = sock.recv(65536)
        if not chunk:
            break
        raw += chunk
        print(f"  Received chunk: {len(chunk)} bytes")
    except socket.timeout:
        print("  Socket timeout (no more data)")
        break

print(f"\nTotal response: {len(raw)} bytes")
print(f"First 2000 chars:")
print(raw[:2000].decode("utf-8", errors="replace"))
print(f"\nLast 500 chars:")
print(raw[-500:].decode("utf-8", errors="replace"))

sock.close()
