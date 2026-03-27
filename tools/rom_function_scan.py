#!/usr/bin/env python3
"""
ROM-based function scanner for Legend of Zelda NES recompilation.

Instead of trying to parse ca65 assembly, this script:
1. Scans the ROM for all JSR instructions and their targets
2. Finds all calls to the TableJump routine ($E5B6 in the disasm)
3. Reads the inline address tables that follow each JSR TableJump
4. Extracts every function address from every jump table
5. Compares against what the recompiler already discovers

This is the ground-truth approach — it reads the actual ROM bytes.
"""

import struct
import sys
import os
from pathlib import Path
from collections import defaultdict

# 6502 opcode info
OPCODE_SIZES = [0] * 256

def _init():
    # Default 1 byte
    for i in range(256): OPCODE_SIZES[i] = 1
    # 2-byte
    for op in [0x09,0x29,0x49,0x69,0xA0,0xA2,0xA9,0xC0,0xC9,0xE0,0xE9,
               0x05,0x06,0x24,0x25,0x26,0x45,0x46,0x65,0x66,0x84,0x85,
               0x86,0xA4,0xA5,0xA6,0xC4,0xC5,0xC6,0xE4,0xE5,0xE6,
               0x15,0x16,0x35,0x36,0x55,0x56,0x75,0x76,0x94,0x95,
               0xB4,0xB5,0xD5,0xD6,0xF5,0xF6,
               0x96,0xB6,
               0x01,0x21,0x41,0x61,0x81,0xA1,0xC1,0xE1,
               0x11,0x31,0x51,0x71,0x91,0xB1,0xD1,0xF1,
               0x10,0x30,0x50,0x70,0x90,0xB0,0xD0,0xF0]:
        OPCODE_SIZES[op] = 2
    # 3-byte
    for op in [0x0D,0x0E,0x20,0x2C,0x2D,0x2E,0x4C,0x4D,0x4E,0x6D,0x6E,
               0x8C,0x8D,0x8E,0xAC,0xAD,0xAE,0xCC,0xCD,0xCE,0xEC,0xED,
               0xEE,0x1D,0x1E,0x3D,0x3E,0x5D,0x5E,0x7D,0x7E,0x9D,0xBD,
               0xBC,0xDD,0xDE,0xFD,0xFE,
               0x19,0x39,0x59,0x79,0x99,0xB9,0xD9,0xF9,0xBE,
               0x6C]:
        OPCODE_SIZES[op] = 3
_init()


def load_banks(banks_dir):
    """Load ROM banks into a dict."""
    banks = {}
    for b in range(8):
        path = os.path.join(banks_dir, f'bank0{b}.bin')
        with open(path, 'rb') as f:
            banks[b] = f.read()
    return banks


def rom_read(banks, bank, addr):
    """Read a byte from ROM at the given address."""
    if addr >= 0xC000:
        # Fixed bank (7)
        offset = addr - 0xC000
        return banks[7][offset]
    elif addr >= 0x8000:
        offset = addr - 0x8000
        return banks[bank][offset]
    return None


def rom_read16(banks, bank, addr):
    """Read a 16-bit LE value from ROM."""
    lo = rom_read(banks, bank, addr)
    hi = rom_read(banks, bank, addr + 1)
    if lo is None or hi is None:
        return None
    return lo | (hi << 8)


def find_tablejump_addr(banks):
    """Find the TableJump routine in the fixed bank.

    TableJump is: ASL A; TAY; PLA; STA $00; PLA; STA $01; INY; LDA ($00),Y; ...
    Bytes: 0A A8 68 85 00 68 85 01 C8 B1 00 ...
    """
    bank7 = banks[7]
    pattern = bytes([0x0A, 0xA8, 0x68, 0x85, 0x00, 0x68, 0x85, 0x01, 0xC8, 0xB1, 0x00])
    offset = bank7.find(pattern)
    if offset >= 0:
        addr = 0xC000 + offset
        print(f"TableJump found at ${addr:04X}", file=sys.stderr)
        return addr
    print("WARNING: TableJump not found!", file=sys.stderr)
    return None


def find_effective_bank(banks, rom_bank, offset, switchbank_addr=0xFFAC):
    """Look backward from a JSR TableJump to find the effective switched bank.

    Searches for the pattern: LDA #imm; JSR SwitchBank ($FFAC)
    within 20 bytes before the call site.

    Returns: the effective bank number, or rom_bank if not found.
    """
    data = banks[rom_bank]
    sb_lo = switchbank_addr & 0xFF
    sb_hi = (switchbank_addr >> 8) & 0xFF

    # Search backward up to 30 bytes for JSR $FFAC
    search_start = max(0, offset - 30)
    for i in range(offset - 3, search_start - 1, -1):
        if (data[i] == 0x20 and data[i+1] == sb_lo and data[i+2] == sb_hi):
            # Found JSR SwitchBank. Now look for LDA #imm before it.
            # Could be immediately before, or separated by other instructions.
            # Check the most common pattern: LDA #imm is at i-2
            if i >= 2 and data[i-2] == 0xA9:  # LDA #imm
                return data[i-1]
            # Also check if there's a PHA; LDA #imm; JSR SwitchBank pattern
            if i >= 3 and data[i-3] == 0xA9:
                return data[i-2]
    return rom_bank


def scan_jsr_tablejump(banks, tablejump_addr):
    """Find all JSR TableJump sites across all banks and extract their inline tables.

    Returns: list of (call_site_bank, call_site_addr, [(target_addr, target_bank), ...])
    """
    results = []
    jsr_opcode = 0x20
    jsr_lo = tablejump_addr & 0xFF
    jsr_hi = (tablejump_addr >> 8) & 0xFF

    for bank in range(8):
        data = banks[bank]
        # Scan through the bank looking for JSR tablejump_addr
        # Fixed bank is at $C000, switchable at $8000
        base_addr = 0xC000 if bank == 7 else 0x8000
        limit = len(data) - 2

        for offset in range(limit):
            if data[offset] == jsr_opcode and data[offset+1] == jsr_lo and data[offset+2] == jsr_hi:
                call_addr = base_addr + offset

                # Determine the effective switchable bank at this call site
                effective_bank = find_effective_bank(banks, bank, offset)

                table_start = call_addr + 3  # Table immediately follows JSR

                # Read entries: each is a 2-byte LE address
                # Table ends when we hit a value < $6000
                entries = []
                tpc = table_start
                while True:
                    target = rom_read16(banks, bank, tpc)
                    if target is None:
                        break
                    if target >= 0xC000:
                        entries.append((target, -1))  # Fixed bank
                    elif target >= 0x8000:
                        entries.append((target, effective_bank))  # Effective switched bank
                    elif 0x6000 <= target < 0x8000:
                        entries.append((target, -2))  # SRAM region
                    else:
                        break  # End of table
                    tpc += 2

                if entries:
                    results.append((bank, call_addr, entries))

    return results


def scan_all_jsr_targets(banks):
    """Find all JSR target addresses across all banks.

    Returns: set of (target_addr, bank) tuples
    """
    targets = set()
    jsr_opcode = 0x20

    for bank in range(8):
        data = banks[bank]
        base_addr = 0xC000 if bank == 7 else 0x8000

        for offset in range(len(data) - 2):
            if data[offset] == jsr_opcode:
                target = data[offset+1] | (data[offset+2] << 8)
                if target >= 0xC000:
                    targets.add((target, -1))
                elif target >= 0x8000:
                    targets.add((target, bank))
                elif 0x6000 <= target < 0x8000:
                    targets.add((target, -2))

    return targets


def load_recompiler_functions(gen_dispatch_path):
    """Parse the generated dispatch file to get currently known functions.

    Returns: set of (addr, bank) tuples
    """
    known = set()
    if not os.path.exists(gen_dispatch_path):
        return known

    # Parse case statements: case 0xXXXX: ... func_XXXX_bY() or func_XXXX()
    import re
    with open(gen_dispatch_path, 'r') as f:
        current_addr = None
        for line in f:
            m = re.match(r'\s*case 0x([0-9A-Fa-f]+):', line)
            if m:
                current_addr = int(m.group(1), 16)

            # Match banked functions: func_XXXX_bY()
            m = re.search(r'func_([0-9A-Fa-f]+)_b(\d+)', line)
            if m:
                addr = int(m.group(1), 16)
                bank = int(m.group(2))
                known.add((addr, bank))
                continue

            # Match fixed-bank functions: func_XXXX() (no bank suffix)
            m = re.search(r'func_([0-9A-Fa-f]+)\(\)', line)
            if m:
                addr = int(m.group(1), 16)
                if addr >= 0xC000:
                    known.add((addr, 7))  # Fixed bank

    return known


def main():
    base_dir = Path(__file__).parent.parent
    banks_dir = base_dir / "banks"
    gen_dispatch = base_dir / "generated" / "zelda_dispatch.c"

    banks = load_banks(banks_dir)

    # Find TableJump routine
    tj_addr = find_tablejump_addr(banks)
    if tj_addr is None:
        sys.exit(1)

    # Scan for all JSR TableJump call sites and their inline tables
    print(f"\nScanning for JSR ${tj_addr:04X} (TableJump) call sites...", file=sys.stderr)
    tablejump_sites = scan_jsr_tablejump(banks, tj_addr)

    print(f"\n{'='*70}")
    print(f"TABLEJUMP DISPATCH TABLES IN ROM")
    print(f"{'='*70}")

    all_table_targets = set()
    for bank, call_addr, entries in sorted(tablejump_sites):
        bank_label = f"bank {bank}" if bank < 7 else "fixed"
        print(f"\n  JSR TableJump at ${call_addr:04X} ({bank_label}): {len(entries)} entries")
        for i, (target, tbank) in enumerate(entries):
            tbank_label = "fixed" if tbank == -1 else f"SRAM" if tbank == -2 else f"bank {tbank}"
            print(f"    [{i:2d}] ${target:04X} ({tbank_label})")
            all_table_targets.add((target, tbank))

    # Scan all JSR targets
    print(f"\n{'='*70}")
    print(f"ALL JSR TARGETS (direct calls)")
    print(f"{'='*70}")
    jsr_targets = scan_all_jsr_targets(banks)
    print(f"  Total unique JSR targets: {len(jsr_targets)}")

    # Combine: all functions = JSR targets + TableJump table entries
    all_functions = jsr_targets | all_table_targets
    print(f"  Total unique function addresses (JSR + tables): {len(all_functions)}")

    # Separate by bank
    by_bank = defaultdict(set)
    for addr, bank in all_functions:
        by_bank[bank].add(addr)

    for bank in sorted(by_bank.keys()):
        bank_label = "fixed" if bank == -1 else "SRAM" if bank == -2 else f"bank {bank}"
        print(f"    {bank_label}: {len(by_bank[bank])} functions")

    # Load recompiler's known functions
    known = load_recompiler_functions(gen_dispatch)
    print(f"\n  Recompiler currently knows: {len(known)} function entries")

    # Find gaps
    # For table targets, we know the exact bank
    # For JSR targets, the bank depends on context
    print(f"\n{'='*70}")
    print(f"TABLE JUMP TARGETS NOT IN RECOMPILER")
    print(f"{'='*70}")

    table_targets_normalized = set()
    for addr, bank in all_table_targets:
        if bank == -1:
            # Fixed bank targets - recompiler uses bank 7
            table_targets_normalized.add((addr, 7))
        elif bank == -2:
            # SRAM - translate to ROM (bank 1, offset +0x3870)
            rom_addr = addr - 0x6C90 + 0xA500
            table_targets_normalized.add((rom_addr, 1))
        else:
            table_targets_normalized.add((addr, bank))

    missing_from_tables = table_targets_normalized - known
    present = table_targets_normalized & known

    print(f"  Table targets found: {len(table_targets_normalized)}")
    print(f"  Already in recompiler: {len(present)}")
    print(f"  MISSING from recompiler: {len(missing_from_tables)}")

    if missing_from_tables:
        print(f"\n  Missing entries (extra_func format):")
        for addr, bank in sorted(missing_from_tables):
            if 0xC000 <= addr <= 0xFFFF:
                print(f"    extra_func -1 {addr:04X}")
            else:
                print(f"    extra_func {bank} {addr:04X}")

    # Output complete extra_func list for ALL table targets
    print(f"\n{'='*70}")
    print(f"COMPLETE extra_func LIST (all TableJump targets)")
    print(f"{'='*70}")
    for addr, bank in sorted(table_targets_normalized):
        if 0xC000 <= addr <= 0xFFFF:
            print(f"extra_func -1 {addr:04X}")
        else:
            print(f"extra_func {bank} {addr:04X}")

    # Summary of all dispatch tables with entry counts
    print(f"\n{'='*70}")
    print(f"DISPATCH TABLE SUMMARY")
    print(f"{'='*70}")
    total_entries = sum(len(e) for _, _, e in tablejump_sites)
    print(f"  Total TableJump call sites: {len(tablejump_sites)}")
    print(f"  Total dispatch entries: {total_entries}")
    print(f"  Unique target addresses: {len(all_table_targets)}")


if __name__ == '__main__':
    main()
