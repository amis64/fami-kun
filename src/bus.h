#include <stdint.h>
#include "mappers.h"
void bus_init(uint16_t mapper, Cart *cart_ptr);
uint8_t bus_read(uint16_t addr);
void bus_write(uint16_t addr, uint8_t value);
