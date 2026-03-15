"""Analyze bank07.bin (fixed bank at $C000-$FFFF) for MMC1 bank-switch patterns."""
import sys

BANK_BASE = 0xC000
BANK_FILE = "F:/Projects/nesrecomp projects/LegendOfZeldaNESRecomp/banks/bank07.bin"

with open(BANK_FILE, "rb") as f:
    data = f.read()

print(f"Bank07 size: {len(data)} bytes (0x{len(data):X})")
print(f"Address range: ${BANK_BASE:04X}-${BANK_BASE + len(data) - 1:04X}")
print()

# Verify interrupt vectors at end of bank
nmi = data[0x3FFA] | (data[0x3FFB] << 8)
reset = data[0x3FFC] | (data[0x3FFD] << 8)
irq = data[0x3FFE] | (data[0x3FFF] << 8)
print(f"Vectors: NMI=${nmi:04X}, RESET=${reset:04X}, IRQ=${irq:04X}")
print()

# 6502 opcodes
STA_ABS = 0x8D  # STA absolute
LSR_A = 0x4A    # LSR A
RTS = 0x60
JSR = 0x20
JMP_ABS = 0x4C
JMP_IND = 0x6C
PLA = 0x68
PHA = 0x48
TAX = 0xAA
TAY = 0xA8
INY = 0xC8
LDA_ABS_Y = 0xB9
STA_ZP = 0x85
LDA_ZP = 0xA5

# Search for MMC1 shift register write pattern:
# 5 sequential STA to mapper registers ($8000-$FFFF) with LSR A between
print("=== Searching for MMC1 shift register sequences ===")
print("Pattern: STA $xxxx; LSR A; STA $xxxx; LSR A; ... (5 writes)")
print()

for i in range(len(data) - 20):
    # Look for sequence of STA abs + LSR A, at least 4 times
    writes = 0
    pos = i
    targets = []
    while pos < len(data) - 3:
        if data[pos] == STA_ABS:
            addr = data[pos+1] | (data[pos+2] << 8)
            if addr >= 0x8000:
                targets.append(addr)
                writes += 1
                pos += 3
                if pos < len(data) and data[pos] == LSR_A:
                    pos += 1
                else:
                    break
            else:
                break
        else:
            break

    if writes >= 4:
        func_addr = BANK_BASE + i
        print(f"  Found {writes}-write sequence at ${func_addr:04X} (offset 0x{i:04X})")
        print(f"  Targets: {', '.join(f'${t:04X}' for t in targets)}")
        # Show surrounding context
        start = max(0, i - 16)
        end = min(len(data), i + 30)
        # Find function start (scan back for RTS or start of bank)
        func_start = i
        for j in range(i-1, max(0, i-64), -1):
            if data[j] == RTS or data[j] == 0x40:  # RTS or RTI
                func_start = j + 1
                break
        print(f"  Likely function start: ${BANK_BASE + func_start:04X}")
        # Disassemble a few bytes around
        hexdump = " ".join(f"{data[k]:02X}" for k in range(max(0,i-8), min(len(data), i+25)))
        print(f"  Bytes: {hexdump}")
        print()

# Search for trampoline patterns:
# A trampoline typically: PLA, TAY, PLA, TAX (or STA zp), INY, ... read inline bytes, then JMP
print("=== Searching for trampoline patterns (PLA sequences for inline data) ===")
print()

for i in range(len(data) - 10):
    # Pattern 1: PLA; TAY; PLA; TAX (get return address from stack)
    if data[i] == PLA and data[i+1] in (TAY, TAX, STA_ZP) and (
        (i+2 < len(data) and data[i+2] == PLA)):
        addr = BANK_BASE + i
        hexdump = " ".join(f"{data[k]:02X}" for k in range(i, min(len(data), i+30)))
        print(f"  PLA sequence at ${addr:04X}: {hexdump}")

    # Pattern 2: PLA; STA zp; PLA; STA zp (save return addr to ZP)
    if (data[i] == PLA and data[i+1] == STA_ZP and
        i+4 < len(data) and data[i+3] == PLA and data[i+4] == STA_ZP):
        addr = BANK_BASE + i
        zp1 = data[i+2]
        zp2 = data[i+5]
        hexdump = " ".join(f"{data[k]:02X}" for k in range(i, min(len(data), i+40)))
        print(f"  PLA;STA ${zp1:02X};PLA;STA ${zp2:02X} at ${addr:04X}: {hexdump}")

print()

# Search for JSR targets that might be trampolines in the fixed bank
# Look at what the switchable banks JSR to in the $C000+ range
print("=== Reset handler disassembly (first 64 bytes from RESET vector) ===")
reset_off = reset - BANK_BASE
if 0 <= reset_off < len(data):
    for j in range(min(64, len(data) - reset_off)):
        if j % 16 == 0:
            print(f"  ${reset + j:04X}: ", end="")
        print(f"{data[reset_off + j]:02X} ", end="")
        if j % 16 == 15:
            print()
    print()
print()

# Also check NMI handler
print(f"=== NMI handler (first 64 bytes from ${nmi:04X}) ===")
nmi_off = nmi - BANK_BASE
if 0 <= nmi_off < len(data):
    for j in range(min(64, len(data) - nmi_off)):
        if j % 16 == 0:
            print(f"  ${nmi + j:04X}: ", end="")
        print(f"{data[nmi_off + j]:02X} ", end="")
        if j % 16 == 15:
            print()
    print()
