#pragma once
#include <stdint.h>

typedef struct{
    uint8_t ppuctrl;
    uint8_t ppumask;
    uint8_t ppustatus;
    uint8_t oamaddr;
    uint8_t ppudata_buffer;
    uint16_t v;
    uint16_t t;
    uint8_t x;
    uint8_t w;
    int oam[256];
    
}Ppu;
void write_ppu_registers(uint16_t addr, uint8_t value, Ppu *ppu);
uint8_t read_ppu_registers(uint16_t addr, Ppu *ppu);
