#include "cartridge.h"
#include <stdio.h>
#include <stdint.h>

typedef struct{
    uint8_t (*mapper_read)(uint16_t addr);
    void (*mapper_write)(uint16_t addr, uint8_t value);
}Mapper;

Mapper mapper_init(uint16_t mapper);

//---MAPPER 0----
uint8_t mapper_0_read(uint16_t addr);
void mapper_0_write(uint16_t addr, uint8_t value);

