#include <stdio.h>
#include <stdint.h>

#pragma once

typedef struct{
    uint8_t *chr_rom;
    int prg_rom;
    int* prg_ram;
    unsigned char nametable_arg;

}Cart;
