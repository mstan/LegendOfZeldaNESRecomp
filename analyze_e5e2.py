"""Analyze the inline dispatch at $E5E2 and search for more dispatch patterns."""
BANK_BASE = 0xC000

with open("F:/Projects/nesrecomp projects/LegendOfZeldaNESRecomp/banks/bank07.bin", "rb") as f:
    bank7 = f.read()

# Show bytes around $E5E2
off = 0xE5E2 - BANK_BASE
print(f"=== Raw bytes at $E5E0-$E600 ===")
for i in range(off - 2, min(off + 30, len(bank7))):
    addr = BANK_BASE + i
    if (i - off + 2) % 16 == 0:
        print(f"\n  ${addr:04X}: ", end="")
    print(f"{bank7[i]:02X} ", end="")
print("\n")

# Simple disasm of $E5E0-$E5F6
opcodes = {
    0x00: ("BRK", 1), 0x08: ("PHP", 1), 0x0A: ("ASL A", 1),
    0x18: ("CLC", 1), 0x20: ("JSR $%04X", 3), 0x28: ("PLP", 1),
    0x29: ("AND #$%02X", 2), 0x38: ("SEC", 1), 0x48: ("PHA", 1),
    0x4A: ("LSR A", 1), 0x4C: ("JMP $%04X", 3), 0x60: ("RTS", 1),
    0x65: ("ADC $%02X", 2), 0x68: ("PLA", 1), 0x69: ("ADC #$%02X", 2),
    0x6C: ("JMP ($%04X)", 3), 0x78: ("SEI", 1), 0x85: ("STA $%02X", 2),
    0x86: ("STX $%02X", 2), 0x88: ("DEY", 1), 0x8D: ("STA $%04X", 3),
    0x98: ("TYA", 1), 0xA0: ("LDY #$%02X", 2), 0xA2: ("LDX #$%02X", 2),
    0xA5: ("LDA $%02X", 2), 0xA9: ("LDA #$%02X", 2), 0xAA: ("TAX", 1),
    0xA8: ("TAY", 1), 0xB1: ("LDA ($%02X),Y", 2), 0xC8: ("INY", 1),
    0xC9: ("CMP #$%02X", 2), 0xCA: ("DEX", 1), 0xD0: ("BNE $%04X", 2),
    0xE8: ("INX", 1), 0xEA: ("NOP", 1), 0xF0: ("BEQ $%04X", 2),
    0x99: ("STA $%04X,Y", 3), 0x0D: ("ORA $%04X", 3),
}

print(f"=== Disassembly $E5E0-$E5F8 ===")
pos = off - 2
for _ in range(20):
    if pos >= len(bank7):
        break
    op = bank7[pos]
    addr = BANK_BASE + pos
    if op in opcodes:
        fmt, size = opcodes[op]
        if size == 1:
            print(f"  ${addr:04X}: {fmt}")
        elif size == 2:
            val = bank7[pos + 1]
            if op in (0xD0, 0xF0, 0x10, 0x30, 0x90, 0xB0):
                target = addr + 2 + (val if val < 128 else val - 256)
                print(f"  ${addr:04X}: {fmt.split('$')[0]}${target:04X}")
            else:
                print(f"  ${addr:04X}: {fmt % val}")
        elif size == 3:
            val = bank7[pos+1] | (bank7[pos+2] << 8)
            print(f"  ${addr:04X}: {fmt % val}")
        pos += size
    else:
        print(f"  ${addr:04X}: .db ${op:02X}")
        pos += 1
print()

# Now find all callers of $E5E2 and what inline data follows
print("=== Callers of JSR $E5E2 with inline data ===")
# JSR $E5E2 = 20 E2 E5
for i in range(len(bank7) - 4):
    if bank7[i] == 0x20 and bank7[i+1] == 0xE2 and bank7[i+2] == 0xE5:
        addr = BANK_BASE + i
        # Inline data is 2 bytes after JSR (at i+3, i+4)
        if i + 4 < len(bank7):
            target_lo = bank7[i+3]
            target_hi = bank7[i+4]
            target = target_lo | (target_hi << 8)
            # Show context before (what bank was switched to)
            ctx_start = max(0, i - 8)
            ctx_bytes = " ".join(f"{bank7[j]:02X}" for j in range(ctx_start, i+5))
            print(f"  ${addr:04X}: JSR $E5E2  inline_target=${target:04X}  bytes: {ctx_bytes}")

print()

# Search for JMP ($xxxx) patterns (indirect jumps) in bank07
print("=== Indirect JMP ($xxxx) in bank07 ===")
for i in range(len(bank7) - 2):
    if bank7[i] == 0x6C:
        addr_lo = bank7[i+1]
        addr_hi = bank7[i+2]
        target = addr_lo | (addr_hi << 8)
        addr = BANK_BASE + i
        print(f"  ${addr:04X}: JMP (${target:04X})")

print()

# Search for RTS-based dispatch (push addr-1 onto stack, then RTS)
# Pattern: lookup table of (address-1) values, push hi then lo, RTS
print("=== Looking for split hi/lo jump tables in bank07 ===")
# Look for LDA table,X / PHA / LDA table,X / PHA / RTS pattern
for i in range(len(bank7) - 10):
    # Pattern: LDA $xxxx,X; PHA; LDA $xxxx,X; PHA; RTS
    # BD xx xx 48 BD xx xx 48 60
    if (bank7[i] == 0xBD and bank7[i+3] == 0x48 and
        bank7[i+4] == 0xBD and bank7[i+7] == 0x48 and
        bank7[i+8] == 0x60):
        addr = BANK_BASE + i
        hi_table = bank7[i+1] | (bank7[i+2] << 8)
        lo_table = bank7[i+5] | (bank7[i+6] << 8)
        print(f"  ${addr:04X}: LDA ${hi_table:04X},X; PHA; LDA ${lo_table:04X},X; PHA; RTS")
    # Also check: LDA $xxxx,Y; PHA; LDA $xxxx,Y; PHA; RTS
    # B9 xx xx 48 B9 xx xx 48 60
    if (bank7[i] == 0xB9 and bank7[i+3] == 0x48 and
        bank7[i+4] == 0xB9 and bank7[i+7] == 0x48 and
        bank7[i+8] == 0x60):
        addr = BANK_BASE + i
        hi_table = bank7[i+1] | (bank7[i+2] << 8)
        lo_table = bank7[i+5] | (bank7[i+6] << 8)
        print(f"  ${addr:04X}: LDA ${hi_table:04X},Y; PHA; LDA ${lo_table:04X},Y; PHA; RTS")
