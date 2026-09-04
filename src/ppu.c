#include "ppu.h"
#include <stdint.h>

uint8_t io_latch;
 uint8_t vram[16384];
void write_ppu_registers(uint16_t addr, uint8_t value, Ppu *ppu){
    addr = 0x2000 + ((addr - 0x2000)%7);
    io_latch = value;
    switch (addr) {
        case 0x2000:
            ppu->ppuctrl = value & 0xFC;
            ppu->t = (ppu->t & 0xE7FF) | (value & 0x03) << 0xA;
            break;

        case 0x2001:
            ppu->ppumask = value;
            break;

        case 0x2002:
            break;

        case 0x2003:
            ppu->oamaddr = value;
            break;

        case 0x2004:
            ppu->oam[ppu->oamaddr] = value;
            ppu->oamaddr++;
            break;

        case 0x2005:
            if(ppu->w){
                ppu->t = ppu->t & 0xC1F;
                ppu->t = ppu->t | (value & 0x3F) << 2;
                ppu->t = ppu->t | (value & 0x07) << 12;
                ppu->w = 0;
            }else{
                ppu->t = (ppu->t & 0x7FE) | (value & 0xF8) >> 3;
                ppu->x = (ppu->x & 0xFF) | (value &  0x07);
                ppu->w = 1;
            }
            break;

        case 0x2006:
            if(ppu->w){
                ppu->t = (ppu->t & 0x7F00) | value;
                ppu->w = 0;
                ppu->v = ppu->t;
            }else{
                ppu->t = (ppu->t & 0xFF) | (value & 0x3F) << 8;
                ppu->w = 1;
            }
            break;

        case 0x2007:
            vram[ppu->v] = value;
            if((ppu->ppuctrl & 0x04) == 0x04){
                ppu->v=+32;
            } else{
                ppu->v++;
            }
    }

}
uint8_t read_ppu_registers(uint16_t addr, Ppu *ppu){
    addr = 0x2000 + ((addr - 0x2000)%7);
    uint8_t read_value;
    switch (addr) {
        case 0x2000:
            read_value = io_latch;
            break;

        case 0x2001:
            read_value = io_latch;
            break;

        case 0x2002:
            ppu->w = 0;
            read_value = (ppu->ppustatus & 0xE0) | (io_latch & 0x1F);
            ppu->ppustatus = ppu->ppustatus & 0x7F;
            io_latch = read_value;
            break;

        case 0x2003:
            read_value = io_latch;
            break;

        case 0x2004:
            //If rendering enabled read oam according to current dot
            read_value = ppu->oam[ppu->oamaddr]; //Behaviour only if rendering is disabled
            break;

        case 0x2005:
            read_value = io_latch;
            break;

        case 0x2006:
            read_value = io_latch;
            break;

        case 0x2007:
            read_value = ppu->ppudata_buffer;
            ppu->ppudata_buffer = vram[ppu->v];
            break;
    }
    return read_value;
}
