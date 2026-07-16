#include <stdint.h>
#include "bus.h"

uint8_t wram[2048];
uint8_t vram[2048];
Mapper current_mapper;
Cart *current_cart;

void bus_init(uint16_t mapper, Cart *cart_ptr){
    current_mapper = mapper_init(mapper);
    current_cart = cart_ptr;
}
uint8_t bus_read(uint16_t addr){
    uint8_t read_value;
    uint16_t ppu_reg;
    //read from wram
    switch (addr){
        case 0x0000 ... 0x1FFF:
            read_value = wram[addr % 0x800];
            break;
        //read from PPU
        case 0x2000 ... 0x3FFF:
            ppu_reg= 0x2000 + (addr % 8);
            if (ppu_reg == 0x2002) {
                // Fake VBlank flag (Bit 7) so CPU boot loops don't get stuck
                read_value = 0x80;
            } else {
                read_value = 0x00;
            }
            break;
        //read from APU
        case 0x4000 ... 0x4017:
            read_value = 0x00;
            break;
        //read from cartridge
        case 0x4020 ... 0xFFFF:
            read_value = current_mapper.mapper_read(addr, current_cart);
            break;
    }

    return read_value;
}

void bus_write(uint16_t addr, uint8_t value){
    //read from wram
    switch (addr){
        case 0x0000 ... 0x1FFF:
            wram[addr % 0x800] = value;
            break;
        //read from PPU
        case 0x2000 ... 0x3FFF:

            break;
            //read from APU
        case 0x4000 ... 0x4017:
            break;
            //read from cartridge
        case 0x4020 ... 0xFFFF:
            current_mapper.mapper_write(addr, value, current_cart);
            break;
    }

}
