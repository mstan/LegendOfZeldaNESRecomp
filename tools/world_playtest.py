"""Disposable local playtest with prepared states and persistent observations."""
import argparse
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
import json
import os
from pathlib import Path
import shutil
import socket
import subprocess
import threading
import time

PAGE = '''<!doctype html><meta charset="utf-8"><title>Zelda seamless playtest</title>
<style>body{background:#111923;color:#ecf1f5;font:17px system-ui;max-width:900px;margin:36px auto;padding:20px}button,select,textarea{font:inherit;padding:10px;margin:5px;border-radius:7px}button{cursor:pointer}section{background:#1d2a39;padding:18px;margin:16px 0;border-radius:12px}textarea{display:block;width:90%;height:70px}small{color:#bacbda}</style>
<h1>Zelda seamless world — preview</h1>
<p>Use the game window normally. These buttons change its prepared setup; they do not restart the game. Your observations are saved locally.</p>
<button onclick="act({action:'resume'})">Resume game</button><button onclick="act({action:'pause'})">Pause game</button>
<p>Arrows / D-pad: move · Z / bottom face button: sword · X / left face button: item · Enter: inventory · Escape / right shoulder: menu · F7: states · F8: rewind · Mouse wheel: zoom · F4: whole map</p>
<section><h2>1. Overworld and Fit to screen</h2><button onclick="load('overworld')">Load overworld, paused</button>
<p>Resume, watch enemies in the surrounding rooms, then walk through the openings. The camera should follow without stopping to scroll. Resize the window wide and tall: more terrain should appear with the HUD centered. Try the mouse wheel and F4, then return with F4.</p><small>Whole-map simulation currently runs slower. Ordinary neighboring areas stay active ahead of Link.</small><div id="overworld"></div></section>
<section><h2>2. Dungeon key door</h2><button onclick="load('dungeon')">Load Level 1, paused</button>
<p>Resume and walk straight up. The supplied key should open the door. Keep walking into the next room without a scrolling transition. Walk back; the door should remain open. Walk down out of the entrance to test the preserved overworld portal.</p><div id="dungeon"></div></section>
<section><h2>3. Combat and current-build states</h2><button onclick="load('corridor')">Load dungeon corridor, paused</button>
<p>Resume, fight the skeletons with Z, and move between the connected rooms. Use F7 to save/load a spare slot and F8 to rewind. Prepared slots 1–3 match these setups; use slot 4 or higher for your saves.</p><small>Enemy migration and projectile interactions across room boundaries are not complete. Bosses, later dungeons, secrets and both quests still need coverage. Bombable/false walls retain original transitions.</small><div id="combat"></div></section>
<section><h2>4. HUD, inventory and death fixes</h2><button onclick="load('overworld')">Load bomb/inventory setup</button>
<p>Place a bomb with X: its flash should cover the full HUD and world. Open and close inventory with Enter: the current expanded scene should slide down and return without changing aspect or jumping to another area.</p><div id="flash"></div><div id="inventory"></div>
<button onclick="load('death')">Load lethal-contact setup, paused</button><p>Resume: Link starts next to a live enemy with very low health. The enemy must cause the death. The wide view should stay in place through the death fade, Game Over and Continue. Press Enter on Continue.</p><div id="death"></div></section>
<p id="status"></p><script>
const ids=['overworld','dungeon','combat','flash','inventory','death'];
for(const id of ids)document.getElementById(id).innerHTML=`<select id="${id}-result"><option>Deferred</option><option>Pass</option><option>Fail</option></select><textarea id="${id}-note" placeholder="Observations or problems"></textarea><button onclick="record('${id}')">Save observation</button>`;
async function act(body){try{let r=await fetch('/api/action',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify(body)});let j=await r.json();document.getElementById('status').textContent=j.ok?'Saved / ready':j.error;return j}catch(e){document.getElementById('status').textContent=e.message}}
function load(setup){act({action:'setup',setup})}
function record(id){act({action:'record',id,result:document.getElementById(id+'-result').value,note:document.getElementById(id+'-note').value})}
fetch('/api/notes').then(r=>r.json()).then(j=>{for(const id of ids)if(j[id]){document.getElementById(id+'-result').value=j[id].result;document.getElementById(id+'-note').value=j[id].note}});
</script>'''


def main():
    ap=argparse.ArgumentParser()
    for n in ('exe','rom','folder','overworld','dungeon','corridor'):
        ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--port',type=int,default=27585)
    ap.add_argument('--http',type=int,default=27586)
    ap.add_argument('--prepare-only',action='store_true')
    a=ap.parse_args();a.folder.mkdir(parents=True,exist_ok=True)
    env=dict(os.environ,NESRECOMP_NO_LAUNCHER='1')
    for key in ('APPDATA','LOCALAPPDATA','TEMP','TMP'):
        path=a.folder/'profile'/key.lower();path.mkdir(parents=True,exist_ok=True);env[key]=str(path.resolve())
    for key in ('SDL_AUDIODRIVER','SDL_VIDEODRIVER','SDL_RENDER_DRIVER'):env.pop(key,None)
    if a.prepare_only:env.update(SDL_AUDIODRIVER='dummy',SDL_VIDEODRIVER='dummy',SDL_RENDER_DRIVER='software')
    config=a.folder/('prepare.ini' if a.prepare_only else 'config.ini')
    if not config.exists():config.write_text('[Display]\nFullscreen=0\nWindowScale=3\nIntegerScale=0\nVSync=0\n[Audio]\nEnabled=1\nVolume='+('0' if a.prepare_only else '100')+'\n[Launcher]\nSkipLauncher=1\n',encoding='utf-8')
    args=[str(a.exe.resolve()),str(a.rom.resolve()),'--no-save','--tcp',str(a.port),'--config',str(config.resolve())]
    if a.prepare_only:args.append('--hidden')
    si=subprocess.STARTUPINFO();si.dwFlags|=subprocess.STARTF_USESHOWWINDOW;si.wShowWindow=0 if a.prepare_only else 1
    log=(a.folder/('prepare.log' if a.prepare_only else 'game.log')).open('w',encoding='utf-8')
    p=subprocess.Popen(args,cwd=a.exe.parent,env=env,stdout=log,stderr=subprocess.STDOUT,startupinfo=si,creationflags=subprocess.CREATE_NO_WINDOW)
    def command(cmd,**fields):
        with socket.create_connection(('127.0.0.1',a.port),timeout=10) as sock:
            stream=sock.makefile('rwb');stream.write((json.dumps(dict(cmd=cmd,id=1,**fields))+'\n').encode());stream.flush()
            result=json.loads(stream.readline())
            if not result.get('ok'):raise RuntimeError(result)
            return result
    deadline=time.monotonic()+20
    while True:
        if p.poll() is not None:raise RuntimeError(f'game exited {p.returncode}; see {log.name}')
        try:command('menu',open=True);break
        except OSError:
            if time.monotonic()>deadline:raise
            time.sleep(.1)
    command('load_state',path=str(a.overworld.resolve()))
    world=command('zelda_world');assert world['seamless_traversal'], 'Seamless mod did not activate'
    command('window_size',w=1536,h=960)
    setups={'overworld':a.overworld,'dungeon':a.dungeon,'corridor':a.corridor}
    if a.prepare_only:
        command('zelda_world_teleport',room=0x77,x=120,y=189,gear=True)
        dest=a.folder/'setups';dest.mkdir(exist_ok=True)
        command('save_state',path=str((dest/'overworld.state').resolve()))
        shutil.copy2(a.dungeon,dest/'dungeon.state');shutil.copy2(a.corridor,dest/'corridor.state')
        saves=a.exe.parent/'saves';saves.mkdir(exist_ok=True)
        for i,name in enumerate(setups,1):
            slot=saves/(a.rom.stem+f'.slot{i:02d}.cycstate')
            if not slot.exists():shutil.copy2(dest/(name+'.state'),slot)
        command('screenshot',layer='picture',path=str((a.folder/'prepared-overworld.png').resolve()))
        for name,source in (('dungeon',a.dungeon),('corridor',a.corridor)):
            command('load_state',path=str(source.resolve()))
            assert command('zelda_world')['level']==1
            command('screenshot',layer='picture',path=str((a.folder/('prepared-'+name+'.png')).resolve()))
        command('load_state',path=str((dest/'overworld.state').resolve()))
        enemy=next(x for x in command('zelda_world_room',room=0x76)['slots'] if x['type'] and x['hp'] and x['meta']==0)
        command('zelda_world_teleport',room=0x76,x=min(240,enemy['x']),y=max(64,min(239,enemy['y'])),fragile=True)
        command('save_state',path=str((dest/'death.state').resolve()))
        (a.folder/'prepared.json').write_text(json.dumps(dict(world=world,slots=list(setups),native_saves_disabled=True),indent=2)+'\n')
        command('quit');p.wait(timeout=10);log.close();print('Prepared states and mod activation passed');return
    notes_path=a.folder/'observations.json'
    if (a.folder/'setups/death.state').exists():setups['death']=a.folder/'setups/death.state'
    notes=json.loads(notes_path.read_text()) if notes_path.exists() else {}
    class Handler(BaseHTTPRequestHandler):
        def reply(self,data,content='application/json',status=200):
            blob=data.encode() if isinstance(data,str) else json.dumps(data).encode()
            self.send_response(status);self.send_header('Content-Type',content+'; charset=utf-8');self.send_header('Content-Length',str(len(blob)));self.end_headers();self.wfile.write(blob)
        def do_GET(self):
            if self.path=='/':self.reply(PAGE,'text/html')
            elif self.path=='/api/notes':self.reply(notes)
            else:self.reply({'error':'Not found'},status=404)
        def do_POST(self):
            try:
                n=int(self.headers.get('Content-Length','0'));assert 0<n<16384
                req=json.loads(self.rfile.read(n));action=req['action']
                if action=='setup':command('menu',open=True);command('load_state',path=str(setups[req['setup']].resolve()))
                elif action in ('resume','pause'):command('menu',open=action=='pause')
                elif action=='record':
                    assert req['id'] in ('overworld','dungeon','combat','flash','inventory','death') and req['result'] in ('Pass','Fail','Deferred')
                    notes[req['id']]=dict(result=req['result'],note=req.get('note',''),time=time.strftime('%Y-%m-%dT%H:%M:%S%z'))
                    notes_path.write_text(json.dumps(notes,indent=2)+'\n',encoding='utf-8')
                else:raise ValueError('Unknown action')
                self.reply({'ok':True})
            except Exception as e:self.reply({'ok':False,'error':str(e)},status=400)
        def log_message(self,*args):pass
    server=ThreadingHTTPServer(('127.0.0.1',a.http),Handler)
    def watch():p.wait();server.shutdown()
    threading.Thread(target=watch,daemon=True).start()
    (a.folder/'running.json').write_text(json.dumps(dict(helper_pid=os.getpid(),game_pid=p.pid,url=f'http://127.0.0.1:{a.http}/',tcp=a.port),indent=2)+'\n')
    print(f'Ready http://127.0.0.1:{a.http}/',flush=True)
    server.serve_forever();server.server_close();log.close()


if __name__=='__main__':main()
