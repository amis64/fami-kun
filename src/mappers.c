#include <stdint.h>
#include "mappers.h"

Mapper mappers[1] = {
    {mapper_0_read, mapper_0_write},
};

Mapper mapper_init(uint16_t mapper_number){
    return mappers[mapper_number];
}


