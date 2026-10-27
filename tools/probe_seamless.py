"""Actual hidden SDL traversal, adjacent actors and portal preparation probe."""
import argparse
import json
import os
from pathlib import Path
import socket
import subprocess
import time


def main():
    ap=argparse.ArgumentParser()
    for n in ('exe','rom','state','out'):
        ap.add_argument('--'+n,type=Path,required=True)
    ap.add_argument('--port',type=int,default=27583)
    ap.add_argument('--dungeon',action='store_true')
    a=ap.parse_args();a.out.mkdir(parents=True,exist_ok=True)
    env=dict(os.environ,SDL_AUDIODRIVER='dummy',SDL_VIDEODRIVER='dummy',
             SDL_RENDER_DRIVER='software',NESRECOMP_NO_LAUNCHER='1')
    for key in ('APPDATA','LOCALAPPDATA','TEMP','TMP'):
        d=a.out/key.lower();d.mkdir(exist_ok=True);env[key]=str(d.resolve())
    config=a.out/'config.ini';config.write_text('[Display]\nFullscreen=0\nVSync=0\n[Audio]\nEnabled=1\nVolume=0\n',encoding='utf-8')
    kwargs={}
    if os.name=='nt':
        si=subprocess.STARTUPINFO();si.dwFlags|=subprocess.STARTF_USESHOWWINDOW;si.wShowWindow=0
        kwargs=dict(startupinfo=si,creationflags=subprocess.CREATE_NO_WINDOW)
    results={};transcript=[]
    with (a.out/'game.log').open('w',encoding='utf-8') as log:
        p=subprocess.Popen([str(a.exe.resolve()),str(a.rom.resolve()),'--hidden','--no-save',
            '--tcp',str(a.port),'--config',str(config.resolve()),'--world-view','seamless',
            '--world-size','768x480'],cwd=a.exe.parent,env=env,stdout=log,stderr=subprocess.STDOUT,**kwargs)
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
            cmd('menu',open=True);cmd('load_state',path=str(a.state.resolve()))
            cmd('zelda_world_timing',reset=True)
            if not a.dungeon:
                results['initial']=cmd('zelda_world')
                results['neighbor_before']=cmd('zelda_world_room',room=0x76)
                cmd('menu',open=False);wait_frames(45);cmd('menu',open=True)
                results['neighbor_after']=cmd('zelda_world_room',room=0x76)
                assert results['neighbor_after']['ticks']>results['neighbor_before']['ticks']
                assert results['neighbor_after']['slots']!=results['neighbor_before']['slots'],'neighbor did not evolve'
                results['seams']=[]
                for name,room,x,y,button,target in [('east',0x77,240,176,'right',0x78),
                        ('west',0x78,1,176,'left',0x77),('north',0x77,120,64,'up',0x67),
                        ('south',0x67,120,224,'down',0x77)]:
                    edge=cmd('zelda_world_edge',room=room,direction={'right':1,'left':2,'down':4,'up':8}[button])
                    cmd('zelda_world_teleport',room=room,x=edge['x'],y=edge['y'],gear=True)
                    before=cmd('zelda_world');cmd('menu',open=False)
                    cmd('action',name=button,frames=50)
                    positions=[];deadline=time.monotonic()+5
                    while time.monotonic()<deadline:
                        state=cmd('zelda_state');world=cmd('zelda_world_room')
                        positions.append(dict(state=state,world_position=world['world_position']))
                        if state['room']==target:break
                        time.sleep(.015)
                    wait_frames(55);cmd('menu',open=True)
                    after=cmd('zelda_world');state=cmd('zelda_state')
                    shot=cmd('screenshot',layer='picture',path=str((a.out/(name+'.png')).resolve()))
                    passed=state['room']==target and state['mode']==5 and after['handoffs']>before['handoffs']
                    results['seams'].append(dict(name=name,passed=passed,before=before,after=after,state=state,
                        positions=positions,picture=shot))
                    if not passed:raise AssertionError(results['seams'][-1])
                results['final']=cmd('zelda_world')
                assert results['final']['failures']==0,results['final']
                entrance=cmd('zelda_world_entrance',room=0x37)
                cmd('zelda_world_teleport',room=0x37,x=entrance['x'],y=entrance['y'],gear=True)
                cmd('menu',open=False);cmd('action',name='up',frames=45)
                deadline=time.monotonic()+20
                while time.monotonic()<deadline:
                    s=cmd('zelda_state');world=cmd('zelda_world')
                    if world['level']==1 and s['mode']==5:break
                    time.sleep(.06)
                cmd('menu',open=True)
                results['dungeon_entry']=dict(entrance=entrance,state=s,world=world)
                cmd('screenshot',layer='picture',path=str((a.out/'dungeon-entry.png').resolve()))
                assert world['level']==1 and s['mode']==5,results['dungeon_entry']
                cmd('save_state',path=str((a.out/'dungeon-start.state').resolve()))
                results['dungeon_ram']=[cmd('read_ram',addr=address,len=128) for address in (0x687e,0x68fe,0x69fe)]
            results['dungeon_before']=dict(state=cmd('zelda_state'),world=cmd('zelda_world'),keys=cmd('read_ram',addr=0x66e,len=1))
            cmd('menu',open=False);cmd('action',name='up',frames=160)
            positions=[];deadline=time.monotonic()+12
            while time.monotonic()<deadline:
                state=cmd('zelda_state');world=cmd('zelda_world_room')
                positions.append(dict(state=state,world_position=world['world_position'],time=time.monotonic()))
                if state['room']==0x63:break
                time.sleep(.015)
            wait_frames(165);cmd('menu',open=True)
            state=cmd('zelda_state');world=cmd('zelda_world')
            results['key_door']=dict(state=state,world=world,positions=positions,keys=cmd('read_ram',addr=0x66e,len=1))
            cmd('screenshot',layer='picture',path=str((a.out/'key-door.png').resolve()))
            cmd('save_state',path=str((a.out/'key-door.state').resolve()))
            assert state['room']==0x63 and state['mode']==5,results['key_door']
            assert results['key_door']['keys']['hex']=='00',results['key_door']
            assert all(x['state']['mode']==5 for x in positions), 'key doorway entered transition mode'
            results['dungeon_edges']=[]
            for name,room,button,direction,target in [('north',0x63,'up',8,0x53),('south',0x53,'down',4,0x63)]:
                cmd('zelda_world_teleport',room=room,x=120,y=77 if direction==8 else 229)
                cmd('menu',open=False);cmd('action',name=button,frames=45)
                wait_frames(50);cmd('menu',open=True)
                state=cmd('zelda_state')
                results['dungeon_edges'].append(dict(name=name,state=state))
                assert state['room']==target and state['mode']==5,results['dungeon_edges'][-1]
            # Walk out through the actual entrance, preserving its native portal.
            cmd('zelda_world_teleport',room=0x73,x=120,y=213)
            cmd('menu',open=False);cmd('action',name='down',frames=70)
            deadline=time.monotonic()+20
            while time.monotonic()<deadline:
                state=cmd('zelda_state');world=cmd('zelda_world')
                if world['level']==0 and state['mode']==5:break
                time.sleep(.06)
            cmd('menu',open=True)
            results['dungeon_exit']=dict(state=state,world=world)
            assert world['level']==0 and state['room']==0x37 and state['mode']==5,results['dungeon_exit']
            assert world['failures']==0,world
            results['timing']=cmd('zelda_world_timing')
            results['status']='pass'
            cmd('quit');p.wait(timeout=15)
        finally:
            if stream:stream.close()
            if sock:sock.close()
            if p.poll() is None:p.terminate();p.wait(timeout=10)
            (a.out/'results.json').write_text(json.dumps(results,indent=2)+'\n',encoding='utf-8')
            (a.out/'tcp.json').write_text(json.dumps(transcript,indent=2)+'\n',encoding='utf-8')
    print(json.dumps({k:v for k,v in results.items() if k not in ('seams','neighbor_before','neighbor_after')}))


if __name__=='__main__':main()
