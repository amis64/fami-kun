#include <stddef.h>
#include <stdint.h>
#pragma once

typedef enum {ACC, ABS, ABS_X, ABS_Y, IMM, IMP, IND, IND_X, IND_Y, REL, ZP, ZP_X, ZP_Y}Addressin_mode;

typedef struct {
    uint16_t address;
    uint8_t  value;
    int8_t   offset;
} Operands;

typedef struct{
    Addressin_mode mode;
    size_t bytes;
    void (*implementation)(Operands);
    int cycles;
}Instruction;


#define OPCODE_TABLE_SIZE 256
extern const Instruction opcodes[OPCODE_TABLE_SIZE];

