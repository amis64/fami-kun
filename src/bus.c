#include <stdint.h>


uint8_t dummy_memory[0x10000];
//uint8_t ram[0x800];
//uint8_t ppu_registers[0x8];
//uint8_t apu_io[0x20];

uint8_t read_mem(uint16_t addr){
    uint8_t value = dummy_memory [addr];
    return value;
}

void write_mem(uint16_t addr, uint8_t value){
    dummy_memory[addr] = value;
}
