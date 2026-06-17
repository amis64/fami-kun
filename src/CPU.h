#include <stdint.h>
#include <stddef.h>

#pragma once

typedef struct{
    uint8_t accumulator;
    uint8_t x_register;
    uint8_t y_register;
    uint16_t pc;
    uint16_t sp;
    uint8_t status_flags;
}CPU;

typedef enum {ACC, ABS, ABS_X, ABS_Y, IMM, IMP, IND, IND_X, IND_Y, REL, ZP, ZP_X, ZP_Y}Addressin_mode;

typedef struct {
    uint16_t address;
    uint8_t  value;
    int8_t   offset;
} Operands;

typedef struct{
    Addressin_mode mode;
    size_t bytes;
    void (*implementation)(Operands *oper, CPU *cpu);
    int cycles;
}Instruction;


#define OPCODE_TABLE_SIZE 256
extern const Instruction opcodes[OPCODE_TABLE_SIZE];


CPU cpu_init();
void cpu_execute(CPU *cpu_ptr);
Operands fetch_operands(Addressin_mode mode, CPU *cpu_ptr);

void ADC(Operands *oper, CPU *cpu);
void AND(Operands *oper, CPU *cpu);
