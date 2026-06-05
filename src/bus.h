#include <stdint.h>
void bus_init(uint16_t mapper);
uint8_t read(uint16_t addr);
void write(uint16_t addr, uint8_t value);
