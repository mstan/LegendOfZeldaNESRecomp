"""Exercise real stopwatch pickups and scoped native enemy movement over TCP.

Uses a disposable build/catalog and profile; native saves are disabled. The
supplied gameplay state is only a starting fixture. RAM changes place a native
clock pickup; they never set InvClock or the mod's captured area.
"""
import argparse
import json
import os
from pathlib import Path
import socket
import struct
import subprocess
import time

from savestate_probe import records


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    for name in ('exe', 'rom', 'state', 'out'):
        ap.add_argument('--' + name, type=Path, required=True)
    ap.add_argument('--port', type=int, default=27604)
    a = ap.parse_args()
    a.out.mkdir(parents=True, exist_ok=True)
    state = a.exe.parent / 'mods/state.toml'
    assert a.out.resolve().parent in state.resolve().parents, 'Use a disposable build beside the output directory'
    original = state.read_bytes() if state.exists() else None
    try:
        run(a)
    finally:
        if original is None:
            state.unlink(missing_ok=True)
        else:
            state.write_bytes(original)


def run(a):
    results = {}
    for scope in ('entire-map', 'current-viewport', 'current-room'):
        out = (a.out / scope).resolve()
        out.mkdir(exist_ok=True)
        state = a.exe.parent / 'mods/state.toml'
        state.write_text('format_version = 1\n[[feature]]\n'
                         'package_id = "legend-of-zelda.enhancement.seamless-world"\n'
                         'id = "seamless-world"\nenabled = true\n' +
                         (f'[feature.values]\nstopwatch-scope = "{scope}"\n' if scope != 'entire-map' else ''), encoding='utf-8')
        env = dict(os.environ, NESRECOMP_NO_LAUNCHER='1', SDL_AUDIODRIVER='dummy',
                   SDL_VIDEODRIVER='dummy', SDL_RENDER_DRIVER='software')
        for key in ('APPDATA', 'LOCALAPPDATA', 'TEMP', 'TMP'):
            directory = out / key.lower()
            directory.mkdir(exist_ok=True)
            env[key] = str(directory)
        config = out / 'config.ini'
        config.write_text('[Display]\nFullscreen=0\nVSync=0\n[Audio]\nVolume=0\n'
                          '[Input]\nPlayer1Source=0\nPlayer2Source=0\n', encoding='utf-8')
        with (out / 'game.log').open('w', encoding='utf-8') as log:
            p = subprocess.Popen([str(a.exe.resolve()), str(a.rom.resolve()), '--hidden', '--no-save',
                                  '--tcp', str(a.port), '--config', str(config)], cwd=a.exe.parent,
                                 env=env, stdout=log, stderr=subprocess.STDOUT,
                                 creationflags=getattr(subprocess, 'CREATE_NO_WINDOW', 0))
            transcript = []
            sock = None
            try:
                deadline = time.monotonic() + 20
                while time.monotonic() < deadline:
                    assert p.poll() is None, p.returncode
                    try:
                        sock = socket.create_connection(('127.0.0.1', a.port), timeout=1)
                        break
                    except OSError:
                        time.sleep(.1)
                assert sock is not None, 'TCP startup timed out'
                sock.settimeout(20)
                with sock.makefile('rwb') as stream:
                    def cmd(name, expected_ok=True, **fields):
                        request = dict(cmd=name, id=1, **fields)
                        stream.write((json.dumps(request) + '\n').encode())
                        stream.flush()
                        answer = json.loads(stream.readline())
                        transcript.append(dict(request=request, response=answer))
                        assert answer.get('ok') == expected_ok, answer
                        return answer

                    def frames(count):
                        start = cmd('state')['frame']
                        deadline = time.monotonic() + 15
                        while time.monotonic() < deadline:
                            if cmd('state')['frame'] >= start + count:
                                return
                            time.sleep(.025)
                        raise AssertionError('Frame progress timed out')

                    def patch_ram(source, target, changes):
                        # Find the exact live RAM image in the saved hardware
                        # chunk; no hard-coded C struct offsets or success flags.
                        ram = bytes.fromhex(cmd('read_ram', addr=0, len=2048)['hex'])
                        blob = source.read_bytes()
                        chunks = []
                        for tag, payload in records(blob):
                            if tag == b'HW  ':
                                offset = payload.find(ram)
                                assert offset >= 0, 'Live RAM missing from hardware state'
                                payload = bytearray(payload)
                                for address, value in changes.items():
                                    payload[offset + address] = value
                            chunks.append(struct.pack('<4sI', tag, len(payload)) + payload)
                        target.write_bytes(blob[:32] + b''.join(chunks))

                    def pickup(name):
                        source = out / (name + '-before.state')
                        target = out / (name + '.state')
                        cmd('save_state', path=str(source))
                        ram = bytes.fromhex(cmd('read_ram', addr=0, len=2048)['hex'])
                        changes = {0x70+11: ram[0x70], 0x84+11: ram[0x84]+3,
                                   0x34f+11: 0x60, 0xac+11: 0x21, 0x3a8+11: 0xe0,
                                   0x405+11: 0, 0x492+11: 0, 0x4bf+11: 1}
                        patch_ram(source, target, changes)
                        cmd('load_state', path=str(target))
                        cmd('menu', open=False)
                        frames(6)
                        cmd('menu', open=True)
                        assert int(cmd('read_ram', addr=0x66c, len=1)['hex'], 16), 'Native clock was not picked up'
                        captured = cmd('zelda_world_room')
                        assert captured['stopwatch_captured'], captured
                        return captured

                    cmd('menu', open=True)
                    cmd('load_state', path=str(a.state.resolve()))
                    code = b''.join(bytes.fromhex(cmd('read_ram', addr=base, len=4096)['hex']) for base in (0x6000, 0x7000))
                    assert 0x6000+code.find(bytes.fromhex('99570660a004')) == 0x73b5
                    assert cmd('zelda_world')['seamless_traversal']
                    cmd('window_size', w=2520, h=1080)
                    cmd('zelda_world_teleport', room=0x77, x=120, y=160, gear=True)
                    cmd('menu', open=False)
                    frames(100)
                    cmd('menu', open=True)
                    world = cmd('zelda_world')
                    assert world['size'] == [840, 360], world
                    assert cmd('zelda_world_room')['stopwatch_scope'] == scope
                    baseline = out / 'before.state'
                    cmd('save_state', path=str(baseline))
                    before = {room: cmd('zelda_world_room', room=room) for room in (0x76, 0x77, 0x78, 0x66)}
                    (out / 'before.json').write_text(json.dumps(before, indent=2))
                    # Native dropped clock, item ID 0x21, in slot 11 at Link.
                    captured = pickup('pickup')
                    expected = [*world['camera'], world['camera'][0]+840, world['camera'][1]+296]
                    assert captured['stopwatch_bounds'] == expected, (captured, expected)
                    assert captured['stopwatch_room'] == 0x77
                    frozen = out / 'frozen.state'
                    cmd('save_state', path=str(frozen))
                    observations = []
                    for _ in range(8):
                        cmd('menu', open=False)
                        frames(8)
                        cmd('menu', open=True)
                        observations.append({room: cmd('zelda_world_room', room=room) for room in before})
                    motion = []
                    for room in before:
                        samples = [sample[room]['slots'] for sample in observations]
                        for slot in samples[0]:
                            if slot['type'] not in (7, 8, 9, 10, 14):
                                continue
                            positions = {(sample[slot['slot']-1]['x'], sample[slot['slot']-1]['y']) for sample in samples}
                            x = (room & 15)*256 + slot['x']
                            y = (room >> 4)*176 + slot['y'] - 64
                            left, top, right, bottom = captured['stopwatch_bounds']
                            is_frozen = (scope == 'entire-map' or
                                      (scope == 'current-room' and room == 0x77) or
                                      (scope == 'current-viewport' and x+16 > left and x < right and y+16 > top and y < bottom))
                            if is_frozen:
                                assert len(positions) == 1, (scope, room, slot, positions)
                            motion.append(dict(room=room, slot=slot['slot'], frozen=is_frozen, positions=len(positions)))
                    if scope != 'entire-map':
                        assert any(not x['frozen'] and x['positions'] > 1 for x in motion), motion
                    cmd('window_size', w=600, h=1200)
                    cmd('zelda_world_teleport', room=0x78, x=200, y=160, gear=True)
                    cmd('menu', open=False)
                    frames(6)
                    cmd('menu', open=True)
                    moved = cmd('zelda_world_room')
                    assert moved['stopwatch_bounds'] == captured['stopwatch_bounds']
                    assert moved['stopwatch_room'] == 0x77
                    cmd('load_state', path=str(frozen))
                    restored = cmd('zelda_world_room')
                    assert restored['stopwatch_bounds'] == captured['stopwatch_bounds']
                    assert restored['stopwatch_room'] == 0x77 and restored['stopwatch_captured']
                    # Invalid captured rectangles must be refused before any
                    # machine or room state is changed.
                    blob = frozen.read_bytes()
                    bad_chunks = []
                    for tag, payload in records(blob):
                        if tag == b'MOD ' and payload[:64].split(b'\0', 1)[0] == b'zelda.world.stopwatch':
                            payload = bytearray(payload)
                            payload[64+16:64+20] = payload[64+8:64+12]  # zero-width rectangle
                        bad_chunks.append(struct.pack('<4sI', tag, len(payload)) + payload)
                    invalid = out / 'invalid-clock.state'
                    invalid.write_bytes(blob[:32] + b''.join(bad_chunks))
                    failure = cmd('load_state', expected_ok=False, path=str(invalid))
                    assert "zelda.world.stopwatch" in failure['err'], failure
                    assert cmd('zelda_world_room') == restored
                    # Another real pickup while InvClock is already set must
                    # replace the old area. This room has native octoroks.
                    cmd('window_size', w=2520, h=1080)
                    cmd('zelda_world_teleport', room=0x78, x=120, y=160, gear=True)
                    cmd('menu', open=False)
                    frames(5)
                    cmd('menu', open=True)
                    second = pickup('second-pickup')
                    assert second['stopwatch_room'] == 0x78
                    assert second['stopwatch_bounds'] != captured['stopwatch_bounds']
                    cmd('menu', open=False)
                    frames(25)
                    cmd('menu', open=True)
                    after = cmd('zelda_world_room')
                    enemies = [slot for slot in second['slots'] if slot['type'] in (7, 8, 9, 10)]
                    assert enemies, second
                    for slot in enemies:
                        other = after['slots'][slot['slot']-1]
                        assert (slot['x'], slot['y']) == (other['x'], other['y']), (scope, slot, other)
                    results[scope] = dict(captured=captured, world=world, observations=observations,
                                          motion=motion, moved=moved, restored=restored, second_pickup=second,
                                          frozen_active_enemies=len(enemies))
                    cmd('quit')
                    p.wait(timeout=15)
            finally:
                if sock:
                    sock.close()
                if p.poll() is None:
                    p.terminate()
                    p.wait(timeout=10)
                (out / 'tcp.json').write_text(json.dumps(transcript, indent=2))
                (a.out / 'results.json').write_text(json.dumps(results, indent=2))
        print(scope + ': native movement, repeated pickup, adaptive bounds and save/load passed', flush=True)


if __name__ == '__main__':
    main()
