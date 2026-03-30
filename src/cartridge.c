#include "cartridge.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


Cart init_cartridge(FILE *file){
    Cart cartridge;
    uint8_t header[16];
    bool is_ines = false;
    bool is_nes20 = false;
    uint8_t mapper_high;
    uint8_t mapper_low;

    fread(header, 1, 16, file);
    if (header[0]=='N' && header[1]=='E' && header[2]=='S' && header[3]==0x1A){
        is_ines=true;
    }
    if (is_ines==true && (header[7]&0x0C)==0x08){
        is_nes20=true;
    }
    mapper_high = (header[7] & 0xF0) >> 4;
    mapper_low = (header[6] & 0xF0) >> 4;
    cartridge.mapper = (mapper_high << 4) | (mapper_low & 0x0F);
    unsigned char trainer_flag = (header[6] >> 2) & 1;
    cartridge.battery = (header[6] >> 1) & 1;
    cartridge.nametable_arg = header[6] & 1;

    if(is_nes20==false && header[12] != 0 || header[13] != 0 || header[14] != 0 || header[15] != 0){
        cartridge.mapper = cartridge.mapper & 0x0F;
    }

    if(is_ines){
        ines_init(file, &cartridge, header, trainer_flag);
    }else {
        nes20_init(file, &cartridge, header, trainer_flag);
    }

    return cartridge;
}

void ines_init(FILE *file, Cart *cart, uint8_t *header, unsigned char trainer){

    cart->prg_ram = malloc(8192);
    cart->prg_rom = malloc(header[4] * 16384);
    if(trainer){
        fseek(file, 528, SEEK_SET);
    }else{
        fseek(file, 16, SEEK_SET);
    }
    if(header[5] == 0x00){
        cart->chr_ram = malloc(8192);
    }else{
        fread(cart->prg_rom, header[4] * 16384, 1, file);
        cart->chr_rom = malloc(header[5] * 8192);
        fread(cart->chr_rom, header[5] * 8192, 1, file);
    }


}
void nes20_init(FILE *file, Cart *cart, uint8_t *header, unsigned char trainer){

}
