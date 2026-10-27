import argparse, json, os, struct, subprocess, hashlib
from pathlib import Path
ap=argparse.ArgumentParser()
for k in ('exe','rom','state','out'):ap.add_argument('--'+k,type=Path,required=True)
ap.add_argument('--view',default='seamless')
a=ap.parse_args();a.out.mkdir(exist_ok=True)
start=struct.unpack_from('<I',a.state.read_bytes(),28)[0];middle=start+120;end=start+300
inputs=a.out/'input.txt';inputs.write_text(f'{start} UP\n{start+160} -\n{start+180} A\n{start+200} -\n{start+210} DOWN\n{end} -\n')
env=dict(os.environ,SDL_AUDIODRIVER='dummy',NESRECOMP_NO_LAUNCHER='1')
base=[str(a.exe),str(a.rom),'--no-save','--world-view',a.view,'--world-size','768x480','--input',str(inputs)]
si=subprocess.STARTUPINFO();si.dwFlags|=subprocess.STARTF_USESHOWWINDOW;si.wShowWindow=0
for name,source in [('continuous',a.state),('restored',a.out/'middle.state')]:
    args=base+['--load-state',str(source),'--frames',str(end),'--hash-out',str(a.out/(name+'.hash')),'--wav-out',str(a.out/(name+'.wav')),'--present-out',str(a.out/(name+'.png')),'--save-state',f'{end-1}:{a.out/(name+".state")}']
    if name=='continuous':args+=['--save-state',f'{middle-1}:{a.out/"middle.state"}']
    with (a.out/(name+'.log')).open('w') as log:subprocess.run(args,cwd=a.exe.parent,env=env,stdout=log,stderr=subprocess.STDOUT,startupinfo=si,creationflags=subprocess.CREATE_NO_WINDOW,check=True,timeout=120)
hashes={n:(a.out/(n+'.hash')).read_text().splitlines() for n in ('continuous','restored')}
x=[l for l in hashes['continuous'] if int(l.split()[0])>=middle];y=hashes['restored']
def chunks(path):
    blob=path.read_bytes();out={};i=32
    while i<len(blob):
        tag,n=struct.unpack_from('<4sI',blob,i);i+=8;data=blob[i:i+n];i+=n
        if tag==b'MOD ':tag+=data[:64].rstrip(b'\0')
        out[tag]=data
    return out
states=[chunks(a.out/(n+'.state')) for n in ('continuous','restored')]
different=[k.decode() for k in states[0] if states[0][k]!=states[1][k]]
assert all(k=='TRAC' for k in different),different
assert (a.out/'continuous.png').read_bytes()==(a.out/'restored.png').read_bytes(),'world presentation mismatch'
wav=(a.out/'restored.wav').read_bytes()[44:];full=(a.out/'continuous.wav').read_bytes()[44:];tail=full[-len(wav):]
mem_mismatch=[middle+i for i,(u,v) in enumerate(zip(x,y)) if u.split(' mem=')[1].split()[0]!=v.split(' mem=')[1].split()[0]]
pcm_mismatch=[i for i in range(min(len(wav),len(tail))//2) if wav[i*2:i*2+2]!=tail[i*2:i*2+2]]
result=dict(status='architectural-state-and-presentation-pass',view=a.view,start=start,middle=middle,end=end,frames=len(y),native_hash_exact=x==y,final_architectural_chunks_exact=True,final_world_record_exact=True,presentation_exact=True,different_final_chunks=different,memory_picture_mismatch_frames=mem_mismatch,pcm_exact=wav==tail,pcm_mismatch_samples=len(pcm_mismatch),pcm_first_mismatches=pcm_mismatch[:16],pcm_last_mismatch=pcm_mismatch[-1] if pcm_mismatch else None,restored_pcm_bytes=len(wav))
(a.out/'results.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result))
