import struct, os
os.makedirs('banks', exist_ok=True)
d = open('Zelda # NES.NES', 'rb').read()
for i in range(8):
    open(f'banks/bank{i:02d}.bin', 'wb').write(d[0x10 + i*0x4000 : 0x10 + (i+1)*0x4000])
    print(f'bank{i:02d}.bin: 0x{0x10 + i*0x4000:05X} - 0x{0x10 + (i+1)*0x4000 - 1:05X}')
b7 = d[0x10 + 7*0x4000 : 0x10 + 8*0x4000]
nmi   = struct.unpack_from('<H', b7, 0x3FFA)[0]
reset = struct.unpack_from('<H', b7, 0x3FFC)[0]
irq   = struct.unpack_from('<H', b7, 0x3FFE)[0]
print(f'\nVectors (bank 7 at $C000):')
print(f'  NMI:   ${nmi:04X}')
print(f'  RESET: ${reset:04X}')
print(f'  IRQ:   ${irq:04X}')
