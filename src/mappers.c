#include <stdint.h>
#include "mappers.h"


Mapper mappers[1] = {
    {mapper_0_read, mapper_0_write},
};

Mapper mapper_init(uint16_t mapper_number){
    return mappers[mapper_number];
}


//---MAPPER 0----
uint8_t mapper_0_read(uint16_t addr, Cart *cart_ptr){
    uint8_t read_value;
    switch (addr) {
        case 0x6000 ... 0x7FFF:
            if(cart_ptr->pgr_ram_size < 8192){
                read_value = cart_ptr->prg_ram[(addr-0x6000)%cart_ptr->pgr_ram_size];}
            else{read_value = cart_ptr->prg_ram[addr-0x6000];}
            break;

        case 0x8000 ... 0xFFFF:
            if(cart_ptr->pgr_rom_size == 16384 && addr > 0xBFFF){
                read_value = cart_ptr->prg_rom[(addr-0x8000) % 16384];
            }else{read_value = cart_ptr->prg_rom[(addr-0x8000)];}
            break;
        }
    return read_value;
}

void mapper_0_write(uint16_t addr, uint8_t value, Cart *cart_ptr){
    if(cart_ptr->pgr_ram_size < 8192){
      cart_ptr->prg_ram[(addr-0x6000)%cart_ptr->pgr_ram_size] = value;}
    else{cart_ptr->prg_ram[addr-0x6000] = value;}
}
