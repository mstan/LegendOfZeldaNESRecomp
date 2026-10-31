"""Hidden current-build checks for retained world viewport boundaries."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import socket
import subprocess
import time


def main():
    ap=argparse.ArgumentParser()
    for name in ('exe','rom','state','out'):
        ap.add_argument('--'+name,type=Path,required=True)
    ap.add_argument('--require-reuse',action='store_true')
    a=ap.parse_args();a.out.mkdir(parents=True,exist_ok=True)
    env=dict(os.environ,SDL_AUDIODRIVER='dummy',SDL_VIDEODRIVER='dummy',
             SDL_RENDER_DRIVER='software',NESRECOMP_NO_LAUNCHER='1')
    for key in ('APPDATA','LOCALAPPDATA','TEMP','TMP'):
        p=a.out/key.lower();p.mkdir(exist_ok=True);env[key]=str(p.resolve())
    config=a.out/'config.ini'
    config.write_text('[Display]\nFullscreen=0\nVSync=0\n[Audio]\nEnabled=1\nVolume=0\n[Input]\nPlayer1Source=0\nPlayer2Source=0\n',encoding='utf-8')
    with socket.socket() as s:s.bind(('127.0.0.1',0));port=s.getsockname()[1]
    command=[str(a.exe.resolve()),str(a.rom.resolve()),'--hidden','--no-save',
             '--tcp',str(port),'--config',str(config.resolve()),'--world-view','seamless',
             '--mods-root',str((a.exe.parent/'mods').resolve())]
    kwargs={}
    if os.name=='nt':
        si=subprocess.STARTUPINFO();si.dwFlags|=subprocess.STARTF_USESHOWWINDOW;si.wShowWindow=0
        kwargs=dict(startupinfo=si,creationflags=subprocess.CREATE_NO_WINDOW)
    transcript=[];results=dict(command=command)
    with (a.out/'game.log').open('w',encoding='utf-8') as log:
        p=subprocess.Popen(command,cwd=a.exe.parent,env=env,stdout=log,stderr=subprocess.STDOUT,**kwargs)
        sock=stream=None
        try:
            deadline=time.monotonic()+30
            while time.monotonic()<deadline:
                if p.poll() is not None:raise RuntimeError('game exited during startup')
                try:
                    sock=socket.create_connection(('127.0.0.1',port),timeout=1)
                    sock.settimeout(30);stream=sock.makefile('rwb');break
                except OSError:time.sleep(.05)
            assert stream is not None

            def ask(cmd,optional=False,**fields):
                request=dict(cmd=cmd,id=len(transcript)+1,**fields)
                stream.write((json.dumps(request)+'\n').encode());stream.flush()
                reply=json.loads(stream.readline());transcript.append(dict(request=request,response=reply))
                if not optional:assert reply.get('ok'),reply
                return reply

            def shot(name,comparable=True):
                path=(a.out/(name+'.png')).resolve()
                response=ask('screenshot',layer='picture',path=str(path))
                row=dict(receipt=response,sha256=hashlib.sha256(path.read_bytes()).hexdigest())
                results.setdefault('pictures' if comparable else 'live_pictures',{})[name]=row
                return row['sha256']

            ask('menu',open=True);ask('load_state',path=str(a.state.resolve()))
            ask('window_size',w=2304,h=1440);ask('zelda_world_view',atlas=False)
            ask('zelda_world_teleport',room=0x77,x=120,y=139,gear=True)
            ready=(a.out/'ready.state').resolve();ask('save_state',path=str(ready))
            initial=shot('initial');frame=ask('state')['frame']
            assert ask('zelda_world')['size']==[768,480]
            before=ask('zelda_world_canvas',optional=not a.require_reuse)
            for i in range(4):assert shot('repeat-'+str(i))==initial
            after=ask('zelda_world_canvas',optional=not a.require_reuse)
            if a.require_reuse:
                assert after['builds']==before['builds'] and after['reuses']>before['reuses'],(before,after)
            results['paused_reuse']=dict(before=before,after=after)
            for w,h in ((3840,720),(600,1200),(2304,1440)):
                ask('window_size',w=w,h=h)
                shot('resize-'+str(w)+'x'+str(h))
                assert ask('state')['frame']==frame
                assert ask('zelda_world')['size']==[w//3,h//3]
            assert shot('resize-return')==initial
            ask('load_state',path=str(ready));ask('zelda_world_view',atlas=False)
            assert shot('loaded-fit')==initial
            ask('zelda_world_view',atlas=True)
            assert ask('state')['picture']==[4096,1472]
            shot('atlas-paused')
            results['atlas_before']=ask('zelda_world')
            ask('menu',open=False)
            deadline=time.monotonic()+20
            while ask('state')['frame']<frame+20:
                assert time.monotonic()<deadline,'no gameplay progress'
                time.sleep(.03)
            ask('menu',open=True)
            results['atlas_live']=ask('zelda_world')
            assert results['atlas_live']['simulated_rooms']==128,results['atlas_live']
            live=shot('atlas-live',False)
            cache=ask('zelda_world_canvas',optional=not a.require_reuse)
            if a.require_reuse:assert cache['reuses']>0 and 24117248<=cache['bytes']<=33554432,cache
            results['atlas_cache']=cache
            state=(a.out/'atlas.state').resolve();ask('save_state',path=str(state))
            ask('load_state',path=str(state));ask('zelda_world_view',atlas=True)
            assert shot('atlas-loaded',False)==live,'current-build atlas load changed picture'
            ask('zelda_world_view',atlas=False)
            assert ask('state')['picture']==[768,480]
            shot('fit-after-atlas',False)
            results['status']='pass';ask('quit');p.wait(timeout=15);assert p.returncode==0
        finally:
            if stream:stream.close()
            if sock:sock.close()
            if p.poll() is None:p.terminate();p.wait(timeout=10)
            (a.out/'tcp.json').write_text(json.dumps(transcript,indent=2)+'\n',encoding='utf-8')
            (a.out/'results.json').write_text(json.dumps(results,indent=2)+'\n',encoding='utf-8')
    print(json.dumps(dict(status=results.get('status'),ready_state=str(a.out/'ready.state'))))


if __name__=='__main__':main()
