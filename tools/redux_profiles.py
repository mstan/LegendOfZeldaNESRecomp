#!/usr/bin/env python3
"""Prepare the supported Redux release and eight explicit native inputs."""
import argparse,hashlib,json,zipfile
from pathlib import Path

ARCHIVE_SHA256='c181d4830d23c3cfb9c697ce9629b4267bb39277f0618990bd6cb87441e02902'
PATCHES=[('Zelda1_Redux.ips','04baf0e9834ed5470b719c09d2ce35baf38eb2b281f6326d964c41ff1e4cdb06'),
 ('optional/Like Like Rupees.ips','19e0c7983ea19599caac0f60e61f35adb6b4e73c78ec463318cdca230fbda830'),
 ('optional/Zelda 1 & A New Light Dungeon Themes.ips','db167a4153ccca50ae34842370fe8ca4ae3787b272274675a0dac2e2a5a655be'),
 ('optional/Reworked Title Screen.ips','78978a11a755d9d975a1adaaacc3bb2ed4acedc987a4535eab03bc7bd9a737b2')]
def apply_ips(image,patch):
    if patch[:5]!=b'PATCH':raise ValueError('Invalid release IPS')
    result=bytearray(image);at=5
    while at+3<=len(patch):
        if patch[at:at+3]==b'EOF':
            at+=3
            if at==len(patch):return bytes(result)
            if len(patch)-at!=3:break
            size=int.from_bytes(patch[at:],'big')
            if size!=len(result):raise ValueError('Unexpected release image size')
            return bytes(result)
        offset=int.from_bytes(patch[at:at+3],'big');at+=3
        if len(patch)-at<2:break
        size=int.from_bytes(patch[at:at+2],'big');at+=2
        if size:blob=patch[at:at+size];at+=size
        else:
            if len(patch)-at<3:break
            size=int.from_bytes(patch[at:at+2],'big');blob=patch[at+2:at+3]*size;at+=3
        if not size or len(blob)!=size or offset>len(result) or size>len(result)-offset:
            raise ValueError('Release patch exceeds supported cartridge')
        result[offset:offset+size]=blob
    raise ValueError('Truncated release IPS')
def prepare(archive,rom,hd_patch,out):
    source=rom.read_bytes()
    if hashlib.md5(source).hexdigest()!='337bd6f1a1163df31bf2633665589ab0':raise ValueError('Redux needs the original Zelda USA PRG0 ROM')
    if hashlib.sha256(archive.read_bytes()).hexdigest()!=ARCHIVE_SHA256:raise ValueError('Expected the official Redux3.3.3 Zelda1-Redux.zip')
    hd_data=hd_patch.read_bytes()
    if hashlib.sha256(hd_data).hexdigest()!='f6b72eb7f6e1b13c59a7d3bbdebd899d348fbefec557a4946957112e3222e091':raise ValueError('Expected the supported Zelda Remastered1.0.6 ZeldaHD.ips')
    out=out.resolve()
    if out in (rom.resolve(),archive.resolve()) or out in rom.resolve().parents or out in archive.resolve().parents:raise ValueError('Build output must be separate from source inputs')
    out.mkdir(parents=True,exist_ok=True);package=out/'mods/packages/legend-of-zelda.redux/3.3.3';package.mkdir(parents=True,exist_ok=True)
    data=[]
    with zipfile.ZipFile(archive) as z:
        for name,want in PATCHES:
            blob=z.read(name)
            if hashlib.sha256(blob).hexdigest()!=want:raise ValueError('Unexpected release patch '+name)
            data.append(blob);path=package/name;path.parent.mkdir(parents=True,exist_ok=True);path.write_bytes(blob)
        for name in ('ReadMe.md','src/LICENSE'):
            (package/Path(name).name).write_bytes(z.read(name))
    # The unmodified release contains the complete GPL source and its notices.
    # Patch/source assets remain separately licensed package content.
    (package/'Zelda1-Redux-source.zip').write_bytes(archive.read_bytes())
    manifest=Path(__file__).resolve().parents[1]/'mods/optional/legend-of-zelda.redux/3.3.3/manifest.toml'
    (package/'manifest.toml').write_bytes(manifest.read_bytes())
    base=apply_ips(source,data[0]);profiles={}
    for mask in range(8):
        image=base
        for bit in range(3):
            if mask&(1<<bit):image=apply_ips(image,data[bit+1])
        if image[:16]!=bytes.fromhex('4e45531a080012080000700900000000'):raise ValueError('Unexpected Redux cartridge geometry')
        path=out/f'redux-{mask}.nes';path.write_bytes(image)
        profiles[str(mask)]=dict(path=str(path),sha256=hashlib.sha256(image).hexdigest())
    (out/'profiles.json').write_text(json.dumps(profiles,indent=2),encoding='utf-8')
    # Preserve the existing HD-only program alongside stock and Redux. The HD
    # package remains owner-supplied and is still selected through Mods.
    remastered=apply_ips(source,hd_data)
    if remastered[:16]!=source[:16]:raise ValueError('Unexpected Remastered cartridge geometry')
    (out/'remastered.nes').write_bytes(remastered)
    # Same documented gameplay graphics positions in the verified cartridges.
    # Only unambiguous nonblank predecessors can supply existing HD art. New
    # Redux-only and conflicting patterns keep their authored native artwork.
    from collections import defaultdict
    header=['#include <stdint.h>','typedef struct { uint8_t source[16],key[16]; } ZeldaHdAlias;']
    counts=[]
    for mask in range(8):
        image=(out/f'redux-{mask}.nes').read_bytes();mapping=defaultdict(set)
        for first,last in ((0x808f,0x9010),(0xc12b,0xec00)):
            for at in range(first,last-15,16):mapping[image[at:at+16]].add(remastered[at:at+16])
        pairs=sorted((s,next(iter(v))) for s,v in mapping.items() if len(v)==1 and len(set(next(iter(v))))>1 and s!=next(iter(v)))
        header.append(f'static const ZeldaHdAlias zelda_hd_aliases_{mask}[]={{')
        for s,k in pairs:header.append('{{'+','.join(str(b) for b in s)+'},{'+','.join(str(b) for b in k)+'}},')
        header.append('};');counts.append(len(pairs))
    header.append('static const ZeldaHdAlias *const zelda_hd_aliases[]={'+','.join(f'zelda_hd_aliases_{m}' for m in range(8))+'};')
    header.append('static const unsigned zelda_hd_alias_counts[]={'+','.join(map(str,counts))+'};')
    (out/'redux_hd_aliases.h').write_text('\n'.join(header)+'\n')
if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    for name in ('archive','rom','hd-patch','out'):parser.add_argument('--'+name,type=Path,required=True)
    args=parser.parse_args();prepare(args.archive,args.rom,args.hd_patch,args.out)
