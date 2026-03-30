#include <stdio.h>
#include <stdint.h>
#pragma once

typedef struct{
    uint8_t* chr_rom;
    uint8_t* prg_rom;
    uint8_t* prg_ram;
    uint8_t* chr_ram;
    unsigned char nametable_arg;
    unsigned char battery;
    uint8_t mapper;
}Cart;

Cart init_cartridge(FILE *file);
void ines_init(FILE *file, Cart *cart, uint8_t *header, unsigned char trainer);
void nes20_init(FILE *file, Cart *cart, uint8_t *header, unsigned char trainer);
