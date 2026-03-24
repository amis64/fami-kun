#include "cartridge.h"


Cart init_cartridge(FILE *file){
    Cart cartridge;
    unsigned char flag_6;
    unsigned char flag_7;
    //Read INES2.0 flag first eventually

    fseek(file, 6, SEEK_SET);
    fread(&flag_6, 1, 1, file);
    uint8_t mapper_low = (flag_6 & 0xF0) >> 4;
    unsigned char trainer_flag = (flag_6 >> 2) & 1;
    unsigned char prg_ram_flag = (flag_6 >> 1) & 1;
    cartridge.nametable_arg = flag_6 & 1;
    fseek(file, 7, SEEK_SET);
    fread(&flag_7, 1, 1, file);
    uint8_t mapper_high = (flag_7 & 0xF0) >> 4;
    uint8_t mapper = (mapper_high << 4) | (mapper_low & 0x0F);

    return cartridge;
}
