"""Actual hidden SDL traversal, adjacent actors and portal preparation probe."""
import argparse
import json
import os
from pathlib import Path
import socket
import subprocess
import time
from PIL import Image


def main():
    ap=argparse.ArgumentParser()
    for n in ('exe','rom','state','out'):
        ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--port',type=int,default=27583)
    ap.add_argument('--dungeon',action='store_true')
    ap.add_argument('--adaptive',action='store_true')
    a=ap.parse_args();a.out.mkdir(parents=True,exist_ok=True)
    env=dict(os.environ,SDL_AUDIODRIVER='dummy',SDL_VIDEODRIVER='dummy',
             SDL_RENDER_DRIVER='software',NESRECOMP_NO_LAUNCHER='1')
    for key in ('APPDATA','LOCALAPPDATA','TEMP','TMP'):
        d=a.out/key.lower();d.mkdir(exist_ok=True);env[key]=str(d.resolve())
    config=a.out/'config.ini';config.write_text('[Display]\nFullscreen=0\nVSync=0\n[Audio]\nEnabled=1\nVolume=0\n[Input]\nPlayer1Source=0\nPlayer2Source=0\n',encoding='utf-8')
    kwargs={}
    if os.name=='nt':
        si=subprocess.STARTUPINFO();si.dwFlags|=subprocess.STARTF_USESHOWWINDOW;si.wShowWindow=0
        kwargs=dict(startupinfo=si,creationflags=subprocess.CREATE_NO_WINDOW)
    results={};transcript=[]
    with (a.out/'game.log').open('w',encoding='utf-8') as log:
        args=[str(a.exe.resolve()),str(a.rom.resolve()),'--hidden','--no-save',
            '--tcp',str(a.port),'--config',str(config.resolve()),'--world-view','seamless',
            ]
        if not a.adaptive:args+=['--world-size','768x480']
        p=subprocess.Popen(args,cwd=a.exe.parent,env=env,stdout=log,stderr=subprocess.STDOUT,**kwargs)
        stream=None;sock=None
        try:
            deadline=time.monotonic()+15
            while time.monotonic()<deadline:
                if p.poll() is not None:raise RuntimeError(f'game exited {p.returncode}')
                try:
                    sock=socket.create_connection(('127.0.0.1',a.port),timeout=1)
                    sock.settimeout(20);stream=sock.makefile('rwb');break
                except OSError:time.sleep(.1)
            if stream is None:raise RuntimeError('TCP startup timed out')
            def cmd(command,**fields):
                request=dict(cmd=command,id=1,**fields)
                stream.write((json.dumps(request)+'\n').encode());stream.flush()
                reply=json.loads(stream.readline());transcript.append(dict(request=request,response=reply))
                if not reply.get('ok'):raise RuntimeError(reply)
                return reply
            def wait_frames(n):
                start=cmd('state')['frame'];deadline=time.monotonic()+max(10,n/20)
                while time.monotonic()<deadline:
                    s=cmd('state')
                    if s['frame']>=start+n:return s
                    time.sleep(.04)
                raise RuntimeError('frame progress timed out')
            def ui_picture(name,kind,size):
                path=(a.out/(name+'.png')).resolve()
                picture=cmd('screenshot',layer='picture',path=str(path))
                w=cmd('zelda_world')
                assert picture['size']==size and w['presentation']==kind,(picture,w)
                assert w['ui_bounds'][2]>=size[0]*.8,w
                with Image.open(path) as im:
                    rgb=im.convert('RGB');pixels=rgb.load()
                    widest=max(sum(max(pixels[x,y])>40 for x in range(im.width))
                               for y in range(im.height))
                # A broad rule/selection must really be drawn, not just reported
                # as a wide output size around a native page.
                assert widest>=size[0]*.8,(name,widest,size)
                return dict(picture=picture,world=w,widest_content_row=widest)
            cmd('menu',open=True);cmd('load_state',path=str(a.state.resolve()))
            if a.adaptive:cmd('window_size',w=2304,h=1440)
            cmd('screenshot',layer='picture',path=str((a.out/'initial.png').resolve()))
            results['initial']=cmd('zelda_world')
            cmd('menu',open=False);wait_frames(6);cmd('action',name='start',frames=6)
            results['menu']=[]
            for i in range(20):
                wait_frames(5);w=cmd('zelda_world')
                shot=cmd('screenshot',layer='picture',path=str((a.out/f'menu-{i}.png').resolve())) if i in (0,5,10,19) else None
                results['menu'].append(dict(world=w,picture=shot))
                assert w['size']==[768,480],w
            assert any(x['world']['menu']!=0 for x in results['menu'])
            cmd('action',name='start',frames=6);wait_frames(100)
            results['menu_closed']=cmd('zelda_world');assert results['menu_closed']['menu']==0
            assert results['menu_closed']['camera']==results['initial']['camera']
            cmd('menu',open=True);cmd('load_state',path=str(a.state.resolve()))
            if a.dungeon:cmd('zelda_world_teleport',room=0x73,x=120,y=141,gear=True)
            cmd('menu',open=False);wait_frames(8)
            results['bomb_before']=cmd('read_ram',addr=0x658,len=1)
            cmd('action',name='b',frames=6)
            deadline=time.monotonic()+8;results['bomb_frames']=[]
            while time.monotonic()<deadline:
                wait_frames(2);w=cmd('zelda_world');results['bomb_frames'].append(w)
                if w['mask64']&1:
                    cmd('menu',open=True)
                    results['bomb_flash']=cmd('screenshot',layer='picture',path=str((a.out/'bomb-flash.png').resolve()))
                    break
            assert 'bomb_flash' in results, 'No native bomb flash observed'
            results['bomb_after']=cmd('read_ram',addr=0x658,len=1)
            assert int(results['bomb_after']['hex'],16)<int(results['bomb_before']['hex'],16)
            cmd('load_state',path=str(a.state.resolve()))
            cmd('menu',open=False);wait_frames(40);cmd('menu',open=True)
            room=0x63 if a.dungeon else 0x76
            neighbor=cmd('zelda_world_room',room=room)
            enemy=next(x for x in neighbor['slots'] if x['type'] and x['hp'] and x['meta']==0)
            cmd('zelda_world_teleport',room=room,x=min(240,enemy['x']),y=max(64,min(239,enemy['y'])),fragile=True)
            cmd('menu',open=False);results['death']=[];deadline=time.monotonic()+20;died=False
            while time.monotonic()<deadline:
                wait_frames(3);w=cmd('zelda_world')
                if w['game_mode']==0x11:
                    died=True
                    results['death'].append(w)
                    if len(results['death']) in (1,7,15,35,60):cmd('screenshot',layer='picture',path=str((a.out/f'death-{len(results["death"])}.png').resolve()))
                    assert w['size']==[768,480],w
                    if w['presentation']=='game-over' and 'game_over_text' not in results:
                        cmd('menu',open=True)
                        results['game_over_text']=ui_picture('game-over-text','game-over',[768,480])
                        cmd('menu',open=False)
                if died and w['game_mode']==8:
                    wait_frames(25)
                    cmd('menu',open=True)
                    results['game_over']=ui_picture('game-over','continue',[768,480])
                    cmd('save_state',path=str((a.out/'continue.state').resolve()))
                    if a.adaptive:
                        results['continue_resizes']=[]
                        for width,height in ((3840,720),(600,1200),(2304,1440)):
                            cmd('window_size',w=width,h=height)
                            results['continue_resizes'].append(ui_picture(f'continue-{width}x{height}','continue',[width//3,height//3]))
                    # Exercise the native Select state and resulting highlight.
                    cmd('menu',open=False);cmd('action',name='select',frames=6);wait_frames(10)
                    cmd('menu',open=True)
                    results['save_selected']=ui_picture('save-selected','continue',[768,480])
                    assert results['save_selected']['world']['submode']==1
                    cmd('menu',open=False)
                    for _ in range(2):cmd('action',name='select',frames=6);wait_frames(10)
                    break
            assert died and 'game_over' in results,'native lethal contact/death sequence not observed'
            assert 'game_over_text' in results,'actual Game Over text phase not tested'
            cmd('action',name='start',frames=6);results['continue']=[];deadline=time.monotonic()+15
            while time.monotonic()<deadline:
                wait_frames(4);w=cmd('zelda_world');results['continue'].append(w)
                assert w['size']==[768,480],w
                if w['game_mode']==5:break
            assert results['continue'][-1]['game_mode']==5,'Continue did not return to gameplay'
            cmd('menu',open=True);cmd('load_state',path=str((a.out/'continue.state').resolve()))
            cmd('menu',open=False);wait_frames(8)
            for _ in range(2):cmd('action',name='select',frames=6);wait_frames(10)
            assert cmd('zelda_world')['submode']==2,'Retry selection failed'
            cmd('action',name='start',frames=6);deadline=time.monotonic()+8
            while time.monotonic()<deadline:
                wait_frames(5);w=cmd('zelda_world')
                if w['game_mode'] in (0,1):break
            assert w['game_mode'] in (0,1) and w['presentation']=='system-ui',w
            results['retry']=w
            cmd('screenshot',layer='picture',path=str((a.out/'retry.png').resolve()))
            results['timing']=cmd('zelda_world_timing')
            results['status']='pass'
            cmd('quit');p.wait(timeout=15)
        finally:
            if stream:stream.close()
            if sock:sock.close()
            if p.poll() is None:p.terminate();p.wait(timeout=10)
            (a.out/'results.json').write_text(json.dumps(results,indent=2)+'\n',encoding='utf-8')
            (a.out/'tcp.json').write_text(json.dumps(transcript,indent=2)+'\n',encoding='utf-8')
    print(json.dumps(dict(status=results.get('status'),menu_frames=len(results.get('menu',[])),bomb=bool(results.get('bomb_flash')),death_frames=len(results.get('death',[])),game_over=bool(results.get('game_over')))))

if __name__=='__main__':main()
