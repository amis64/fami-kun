#include <stdio.h>
#include <stdint.h>
#pragma once

typedef struct{
    uint8_t* chr_rom;
    uint8_t* prg_rom;
    uint8_t* prg_ram;
    uint8_t* prg_nvram;
    uint8_t* chr_nvram;
     uint8_t* chr_ram;
    unsigned char nametable_arg;
    unsigned char alt_nametable;
    unsigned char battery;
    uint16_t mapper;
    uint16_t submapper;
    int pgr_rom_size;
    int pgr_ram_size;
    int pgr_nvram_size;
    int chr_rom_size;
    int chr_ram_size;
    int chr_nvram_size;
}Cart;

Cart init_cartridge(FILE *file);
void ines_init(FILE *file, Cart *cart, uint8_t *header, unsigned char trainer);
void nes20_init(FILE *file, Cart *cart, uint8_t *header, unsigned char trainer);
