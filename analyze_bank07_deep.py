"""Deep analysis of bank07.bin - find trampoline and all JSR $FFAC references."""
import sys

BANK_BASE = 0xC000

with open("F:/Projects/nesrecomp projects/LegendOfZeldaNESRecomp/banks/bank07.bin", "rb") as f:
    bank7 = f.read()

# Key opcodes
STA_ABS = 0x8D; LSR_A = 0x4A; RTS = 0x60; JSR = 0x20; JMP_ABS = 0x4C
JMP_IND = 0x6C; PLA = 0x68; PHA = 0x48; TAX = 0xAA; TAY = 0xA8
INY = 0xC8; LDA_IMM = 0xA9; LDA_ABS = 0xAD; STA_ZP = 0x85; LDA_ZP = 0xA5
LDA_IND_Y = 0xB1; SEC = 0x38; CLC = 0x18; ADC_IMM = 0x69; SBC_IMM = 0xE9

# === Disassemble the bank-switch functions ===
def disasm_at(data, base, offset, count=20):
    """Simple 6502 disassembler."""
    opcodes = {
        0x00: ("BRK", 1), 0x01: ("ORA ($%02X,X)", 2),
        0x05: ("ORA $%02X", 2), 0x06: ("ASL $%02X", 2),
        0x08: ("PHP", 1), 0x09: ("ORA #$%02X", 2),
        0x0A: ("ASL A", 1), 0x10: ("BPL $%04X", 2),
        0x18: ("CLC", 1), 0x20: ("JSR $%04X", 3),
        0x24: ("BIT $%02X", 2), 0x25: ("AND $%02X", 2),
        0x26: ("ROL $%02X", 2), 0x28: ("PLP", 1),
        0x29: ("AND #$%02X", 2), 0x2A: ("ROL A", 1),
        0x2C: ("BIT $%04X", 3), 0x30: ("BMI $%04X", 2),
        0x38: ("SEC", 1), 0x45: ("EOR $%02X", 2),
        0x46: ("LSR $%02X", 2), 0x48: ("PHA", 1),
        0x49: ("EOR #$%02X", 2), 0x4A: ("LSR A", 1),
        0x4C: ("JMP $%04X", 3), 0x50: ("BVC $%04X", 2),
        0x60: ("RTS", 1), 0x65: ("ADC $%02X", 2),
        0x66: ("ROR $%02X", 2), 0x68: ("PLA", 1),
        0x69: ("ADC #$%02X", 2), 0x6A: ("ROR A", 1),
        0x6C: ("JMP ($%04X)", 3), 0x78: ("SEI", 1),
        0x84: ("STY $%02X", 2), 0x85: ("STA $%02X", 2),
        0x86: ("STX $%02X", 2), 0x88: ("DEY", 1),
        0x8A: ("TXA", 1), 0x8C: ("STY $%04X", 3),
        0x8D: ("STA $%04X", 3), 0x8E: ("STX $%04X", 3),
        0x90: ("BCC $%04X", 2), 0x91: ("STA ($%02X),Y", 2),
        0x95: ("STA $%02X,X", 2), 0x98: ("TYA", 1),
        0x99: ("STA $%04X,Y", 3), 0x9A: ("TXS", 1),
        0x9D: ("STA $%04X,X", 3),
        0xA0: ("LDY #$%02X", 2), 0xA2: ("LDX #$%02X", 2),
        0xA4: ("LDY $%02X", 2), 0xA5: ("LDA $%02X", 2),
        0xA6: ("LDX $%02X", 2), 0xA8: ("TAY", 1),
        0xA9: ("LDA #$%02X", 2), 0xAA: ("TAX", 1),
        0xAC: ("LDY $%04X", 3), 0xAD: ("LDA $%04X", 3),
        0xAE: ("LDX $%04X", 3),
        0xB0: ("BCS $%04X", 2), 0xB1: ("LDA ($%02X),Y", 2),
        0xB5: ("LDA $%02X,X", 2), 0xB9: ("LDA $%04X,Y", 3),
        0xBD: ("LDA $%04X,X", 3),
        0xC0: ("CPY #$%02X", 2), 0xC4: ("CPY $%02X", 2),
        0xC5: ("CMP $%02X", 2), 0xC6: ("DEC $%02X", 2),
        0xC8: ("INY", 1), 0xC9: ("CMP #$%02X", 2),
        0xCA: ("DEX", 1), 0xCC: ("CPY $%04X", 3),
        0xCD: ("CMP $%04X", 3), 0xCE: ("DEC $%04X", 3),
        0xD0: ("BNE $%04X", 2), 0xD5: ("CMP $%02X,X", 2),
        0xD8: ("CLD", 1), 0xD9: ("CMP $%04X,Y", 3),
        0xE0: ("CPX #$%02X", 2), 0xE4: ("CPX $%02X", 2),
        0xE5: ("SBC $%02X", 2), 0xE6: ("INC $%02X", 2),
        0xE8: ("INX", 1), 0xE9: ("SBC #$%02X", 2),
        0xEA: ("NOP", 1), 0xEC: ("CPX $%04X", 3),
        0xED: ("SBC $%04X", 3), 0xEE: ("INC $%04X", 3),
        0xF0: ("BEQ $%04X", 2), 0xF5: ("SBC $%02X,X", 2),
        0xF6: ("INC $%02X,X", 2), 0xF8: ("SED", 1),
        0x40: ("RTI", 1),
    }

    pos = offset
    lines = []
    for _ in range(count):
        if pos >= len(data):
            break
        op = data[pos]
        addr = base + pos
        if op in opcodes:
            fmt, size = opcodes[op]
            if size == 1:
                lines.append(f"  ${addr:04X}: {fmt}")
            elif size == 2:
                if pos + 1 >= len(data):
                    break
                val = data[pos + 1]
                # Handle branch targets
                if op in (0x10, 0x30, 0x50, 0x70, 0x90, 0xB0, 0xD0, 0xF0):
                    target = addr + 2 + (val if val < 128 else val - 256)
                    lines.append(f"  ${addr:04X}: {fmt.split('$')[0]}${target:04X}")
                else:
                    lines.append(f"  ${addr:04X}: {fmt % val}")
            elif size == 3:
                if pos + 2 >= len(data):
                    break
                val = data[pos + 1] | (data[pos + 2] << 8)
                lines.append(f"  ${addr:04X}: {fmt % val}")
            pos += size
        else:
            lines.append(f"  ${addr:04X}: .db ${op:02X}")
            pos += 1
    return "\n".join(lines)

# === Disassemble key functions ===
print("=== PRG Bank-Switch Function at $FFAC ===")
print(disasm_at(bank7, BANK_BASE, 0xFFAC - BANK_BASE, 15))
print()

print("=== Control Register Write at $FF98 ===")
print(disasm_at(bank7, BANK_BASE, 0xFF98 - BANK_BASE, 15))
print()

print("=== CHR Bank Write at $FF7D ===")
print(disasm_at(bank7, BANK_BASE, 0xFF7D - BANK_BASE, 25))
print()

print("=== Potential Trampoline at $E5E4 ===")
print(disasm_at(bank7, BANK_BASE, 0xE5E4 - BANK_BASE, 20))
print()

print("=== Reset handler at $FF50 ===")
print(disasm_at(bank7, BANK_BASE, 0xFF50 - BANK_BASE, 40))
print()

# === Find ALL JSR $FFAC (bank switch calls) in bank07 ===
print("=== All JSR $FFAC calls in bank07 (PRG bank switch) ===")
for i in range(len(bank7) - 2):
    if bank7[i] == JSR and bank7[i+1] == 0xAC and bank7[i+2] == 0xFF:
        addr = BANK_BASE + i
        # Show context: what's before (LDA #xx to set bank number) and after
        context_start = max(0, i - 4)
        context = disasm_at(bank7, BANK_BASE, context_start, 6)
        print(f"\n  At ${addr:04X}:")
        print(context)
print()

# === Find ALL JSR targets in the $C000+ range from bank07 ===
print("=== All JSR targets in $C000+ range (within bank07) ===")
jsr_targets = {}
for i in range(len(bank7) - 2):
    if bank7[i] == JSR:
        target = bank7[i+1] | (bank7[i+2] << 8)
        if target >= 0xC000:
            if target not in jsr_targets:
                jsr_targets[target] = []
            jsr_targets[target].append(BANK_BASE + i)

for target in sorted(jsr_targets.keys()):
    callers = jsr_targets[target]
    print(f"  JSR ${target:04X} called {len(callers)} times from: {', '.join(f'${c:04X}' for c in callers[:8])}")
print()

# === Look for the pattern: LDA #bank; JSR $FFAC; JSR/JMP $xxxx ===
print("=== Bank-switch-then-call patterns (LDA #xx; JSR $FFAC; JSR/JMP $xxxx) ===")
for i in range(len(bank7) - 7):
    if (bank7[i] == LDA_IMM and
        bank7[i+2] == JSR and bank7[i+3] == 0xAC and bank7[i+4] == 0xFF):
        bank_num = bank7[i+1]
        addr = BANK_BASE + i
        next_off = i + 5
        if next_off + 2 < len(bank7):
            if bank7[next_off] == JSR:
                target = bank7[next_off+1] | (bank7[next_off+2] << 8)
                print(f"  ${addr:04X}: LDA #{bank_num:02X}; JSR $FFAC; JSR ${target:04X}")
            elif bank7[next_off] == JMP_ABS:
                target = bank7[next_off+1] | (bank7[next_off+2] << 8)
                print(f"  ${addr:04X}: LDA #{bank_num:02X}; JSR $FFAC; JMP ${target:04X}")
print()

# === Also check trampoline candidate at $F475 ===
print("=== Function at $F475 ===")
print(disasm_at(bank7, BANK_BASE, 0xF475 - BANK_BASE, 30))
