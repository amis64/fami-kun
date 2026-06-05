#include <stdint.h>
#include <stdio.h>
#include "mappers.h"

uint8_t wram[2048];
uint8_t vram[2048];
Mapper current_mapper;

void bus_init(uint16_t mapper_number){
    current_mapper = mapper_init(mapper_number);
}
uint8_t read(uint16_t addr){
    uint8_t read_value;
    //read from wram
    switch (addr){
        case 0x0000 ... 0x1FFF:
            if(addr > 0x07FF){
                read_value = wram[addr % 0x800];
            }else(read_value = wram[addr]);
            break;
        //read from PPU
        case 0x2000 ... 0x3FFF:
            printf("No PPU yet!");
            break;
        //read from APU
        case 0x4000 ... 0x4017:
            printf("No APU yet!");
            break;
        //read from cartridge
        case 0x4020 ... 0xFFFF:
            read_value = current_mapper.mapper_read(addr);
            break;
    }

    return read_value;
}

void write(uint16_t addr, uint8_t value){
    //read from wram
    switch (addr){
        case 0x0000 ... 0x1FFF:
            if(addr > 0x07FF){
                wram[addr % 0x800] = value;
            }else(wram[addr] = value);

        break;
        //read from PPU
        case 0x2000 ... 0x3FFF:
            printf("No PPU yet!");
            break;
            //read from APU
        case 0x4000 ... 0x4017:
            printf("No APU yet!");
            break;
            //read from cartridge
        case 0x4020 ... 0xFFFF:
            current_mapper.mapper_write(addr, value);
            break;
    }

}
