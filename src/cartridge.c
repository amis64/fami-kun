#include "cartridge.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define DEFAULT_RAM_SIZE 8192
#define CHR_ROM_MULT 8192
#define PRG_ROM_MULT 16384

int get_rom_size(uint8_t msb, uint8_t bit, int multiplier, uint8_t *header);
void get_ram_size(int* ram_size, int* nvram_size, uint8_t bit, uint8_t *header);

Cart init_cartridge(FILE *file){
    Cart cartridge;
    uint8_t header[16];
    bool is_ines = false;
    bool is_nes20 = false;
    fread(header, 1, 16, file);
    if (header[0]=='N' && header[1]=='E' && header[2]=='S' && header[3]==0x1A){
        is_ines=true;
    }
    if (is_ines==true && (header[7]&0x0C)==0x08){
        is_nes20=true;
    }
    unsigned char trainer_flag = (header[6] >> 2) & 1;
    cartridge.battery = (header[6] >> 1) & 1;
    cartridge.nametable_arg = header[6] & 1;
    cartridge.alt_nametable = (header[6] & 0x08) >> 3;


    if(is_ines == true && is_nes20 == false){
        ines_init(file, &cartridge, header, trainer_flag);
    }else if(is_nes20){
        nes20_init(file, &cartridge, header, trainer_flag);
    }

    return cartridge;
}

void ines_init(FILE *file, Cart *cart, uint8_t *header, unsigned char trainer){

    cart->chr_nvram = NULL;
    cart->chr_nvram_size = 0;
    cart->chr_ram = NULL;

    cart->mapper = (header[7] & 0xF0) | ((header[6] & 0xF0) >> 4);
    if(header[12] != 0 || header[13] != 0 || header[14] != 0 || header[15] != 0){
        cart->mapper = cart->mapper & 0x0F;
    }
    cart->submapper = 0;

    if(cart->battery == 0){
        cart->pgr_ram_size = DEFAULT_RAM_SIZE;
        cart->prg_ram = malloc(cart->pgr_ram_size);
        cart->prg_nvram = NULL;
    }
    else{
        cart->pgr_nvram_size = DEFAULT_RAM_SIZE;
        cart->prg_nvram = malloc(cart->pgr_nvram_size);
        cart->prg_ram = NULL;
    }

    cart->pgr_rom_size = header[4] * PRG_ROM_MULT;
    cart->prg_rom = malloc(cart->pgr_rom_size);
    if(trainer){
        fseek(file, 528, SEEK_SET);
    }else{
        fseek(file, 16, SEEK_SET);
    }
    fread(cart->prg_rom, cart->pgr_rom_size, 1, file);

    if(header[5] == 0x00){
        cart->chr_rom_size = 0;
        cart->chr_ram_size = DEFAULT_RAM_SIZE;
        cart->chr_ram = malloc(cart->chr_ram_size);
        cart->chr_rom = NULL;
    }else{
        cart->chr_ram_size = 0;
        cart->chr_ram = NULL;
        cart->chr_rom_size = header[5] * CHR_ROM_MULT;
        cart->chr_rom = malloc(cart->chr_rom_size);
        fread(cart->chr_rom, cart->chr_rom_size, 1, file);
    }



}
void nes20_init(FILE *file, Cart *cart, uint8_t *header, unsigned char trainer){
    uint8_t prg_rom_msb = (header[9] & 0xF);
    uint8_t chr_rom_msb = (header[9] & 0xF0) >> 4;


    cart->mapper = ((header[8] & 0x0F) << 8) | (header[7] & 0xF0) | ((header[6] & 0xF0) >> 4);
    cart->submapper = (header[8] & 0xF0) >>4;

    cart->pgr_rom_size = get_rom_size(prg_rom_msb, 4, PRG_ROM_MULT, header);
    cart->prg_rom = malloc(cart->pgr_rom_size);
    if(trainer){
        fseek(file, 528, SEEK_SET);
    }else{
        fseek(file, 16, SEEK_SET);
    }
    fread(cart->prg_rom, cart->pgr_rom_size, 1, file);

    cart->chr_rom_size = get_rom_size(chr_rom_msb, 5, CHR_ROM_MULT, header);
    if(cart->chr_rom_size != 0x00){
        cart->chr_rom = malloc(cart->chr_rom_size);
        fread(cart->chr_rom, cart->chr_rom_size, 1, file);
    }else{cart->chr_rom = NULL;}

    cart->pgr_ram_size = (64 << (header[10] & 0xF));
    if(cart->pgr_ram_size == 0){
        cart->prg_ram = NULL;
    }
    else{
        cart->prg_ram = malloc(cart->pgr_ram_size);
    }
    cart->pgr_nvram_size = (64 << ((header[10] & 0xF0) >> 4));
    if(cart->pgr_nvram_size == 0){
        cart->prg_nvram = NULL;
    }
    else{
        cart->prg_nvram = malloc(cart->pgr_nvram_size);
    }
    cart->chr_ram_size = (64 << (header[11] & 0xF));
    if(cart->chr_ram_size == 0){
        cart->chr_ram = NULL;
    }
    else{
        cart->chr_ram = malloc(cart->chr_ram_size);
    }
    cart->chr_nvram_size = (64 << ((header[11] & 0xF0) >> 4));
    if(cart->chr_nvram_size == 0){
        cart->chr_nvram = NULL;
    }
    else{
        cart->chr_nvram = malloc(cart->chr_nvram_size);
    }
        

}

int get_rom_size(uint8_t msb, uint8_t bit, int multiplier, uint8_t *header){

    if(msb < 0xF){
        return (((msb << 8) | header[bit]) * multiplier);
    }else{
        return ((1 << ((header[bit] & 0XFC) >> 2)) * ((header[bit] & 0x03) * 2+1));
    }
}
