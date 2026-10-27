# fami-kun

A Nintendo Entertainment System (Famicom) emulator written from scratch in C, built as a way to learn how a computer works at the hardware level: instruction decoding, memory mapping and cycle timing.

> **Status:** work in progress. The 6502 CPU core is complete; the PPU (graphics) is under development.

## Features

### CPU (MOS 6502 / Ricoh 2A03)

- All **56 official instructions** across the 13 addressing modes (immediate, zero page, zero page X/Y, absolute, absolute X/Y, indirect, indexed indirect, indirect indexed, relative, accumulator, implied).
- **Table-driven decoder:** a 256-entry opcode table maps each opcode to its implementation, addressing mode, base cycle count and page-crossing penalty.
- **Cycle counting**, including extra cycles for page crossings and taken branches.
- Status flags (N V B D I Z C), stack operations, subroutines and interrupt return.
- Hardware quirks such as the 6502 indirect `JMP` page-boundary bug.

### Memory and cartridges

- CPU memory bus that routes reads and writes to RAM, PPU registers and the cartridge.
- **iNES ROM loader** with NES 2.0 header detection: PRG/CHR ROM and RAM sizes, trainer, battery and nametable mirroring flags.
- **Mapper 0 (NROM)**, including 16 KB PRG ROM mirroring.

### PPU (in progress)

- CPU-mapped PPU registers ($2000–$2007): control, mask, OAM address/data, scroll and VRAM address, with the internal `v`, `t`, `x` and `w` registers.

## Building

Requires `gcc` and `make`.

```bash
cd src
make
```

This produces the `fami-kun` executable. Use `make clean` to remove build files.

## Usage

```bash
./fami-kun path/to/rom.nes
```

The CPU currently starts execution at `$C000`, the entry point used by the `nestest` CPU test ROM in automated mode.

## Project structure

| File | Responsibility |
| --- | --- |
| `famicom.c` | Entry point and main emulation loop |
| `CPU.c` / `CPU.h` | 6502 core: opcode table, addressing modes and instructions |
| `bus.c` / `bus.h` | CPU memory map |
| `cartridge.c` / `cartridge.h` | iNES / NES 2.0 ROM parsing |
| `mappers.c` / `mappers.h` | Cartridge mappers (currently NROM) |
| `ppu.c` / `ppu.h` | Picture Processing Unit (in progress) |

## Roadmap

- [x] 6502 CPU with official instruction set and cycle counts
- [x] iNES / NES 2.0 cartridge loading
- [x] Mapper 0 (NROM)
- [ ] PPU rendering (background and sprites)
- [ ] Display output and input handling
- [ ] Cycle-accurate CPU/PPU synchronization
- [ ] Additional mappers (MMC1, UxROM)
- [ ] APU (audio)

## References

- [NESdev Wiki](https://www.nesdev.org/wiki/Nesdev_Wiki)
- [6502 instruction reference](https://www.masswerk.at/6502/6502_instruction_set.html)

## License

MIT — see [LICENSE](LICENSE).
