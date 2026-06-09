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
    return read_value;
}

void mapper_0_write(uint16_t addr, uint8_t value, Cart *cart_ptr){
    //read
}
