#include <stdint.h>
#include "opcodes.h"

#pragma once

typedef struct{
    uint8_t accumulator;
    uint8_t x_register;
    uint8_t y_register;
    uint16_t pc;
    uint16_t sp;
    uint8_t status_flags;
}CPU;



CPU cpu_init();
void cpu_execute(uint8_t rom [], CPU cpu); //Read instruction from the rom, match opcode to instruction from look up table,  get mode get_mode_operands, pass get mode get_mode_operands to  void (*implementation)(void) from instruction struct
Operands get_mode_operands(uint8_t rom [], Addressin_mode mode, CPU cpu);
