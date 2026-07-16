#include <stdint.h>
#include <stddef.h>

#pragma once

#define FLAG_CARRY     0x01
#define FLAG_ZERO      0x02
#define FLAG_INTERRUPT 0x04
#define FLAG_DECIMAL   0x08
#define FLAG_BREAK     0x10
#define FLAG_OVERFLOW  0x40
#define FLAG_NEGATIVE  0x80

#define FLAG_CARRY_CLEAR     0xFE
#define FLAG_ZERO_CLEAR      0xFD
#define FLAG_INTERRUPT_CLEAR 0xFB
#define FLAG_DECIMAL_CLEAR   0xF7
#define FLAG_BREAK_CLEAR     0xEF
#define FLAG_OVERFLOW_CLEAR  0xBF
#define FLAG_NEGATIVE_CLEAR  0x7F

#define NMI_VECTOR   0xFFFA
#define RESET_VECTOR 0xFFFC
#define IRQ_VECTOR   0xFFFE

typedef struct{
    uint8_t accumulator;
    uint8_t x_register;
    uint8_t y_register;
    uint16_t pc;
    uint8_t sp;
    uint8_t status_flags;
}CPU;

typedef enum {ACC, ABS, ABS_X, ABS_Y, IMM, IMP, IND, IND_X, IND_Y, REL, ZP, ZP_X, ZP_Y}Addressin_mode;

typedef struct {
    uint16_t address;
    uint8_t  value;
    int8_t   offset;
} Operands;

typedef struct{
    void (*implementation)(Operands *oper, CPU *cpu);
    Addressin_mode mode;
    int cycles;
    uint8_t page_cross_penalty;
}Instruction;


#define OPCODE_TABLE_SIZE 256
extern const Instruction opcodes[OPCODE_TABLE_SIZE];


CPU cpu_init();
int cpu_execute(CPU *cpu_ptr);
Operands fetch_operands(Addressin_mode mode, CPU *cpu_ptr, uint8_t *extra_cycles);

