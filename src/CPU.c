#include "CPU.h"
#include "opcodes.h"
#include "bus.c" //TODO: Create bus.h

CPU cpu_init(){
    CPU cpu;
    cpu.accumulator = 0;
    cpu.x_register = 0;
    cpu.y_register = 0;
    cpu.pc = 0;
    cpu.sp = 0;
    cpu.status_flags = 0b00000000;
    return cpu;
}
uint16_t get_address(uint8_t rom [], uint16_t pc);

Operands get_mode_operands(uint8_t rom [], Addressin_mode mode, CPU cpu){
    Operands operands;

    operands.address = 0;
    operands.value = 0;
    operands.offset = 0;


    switch (mode) {

        case IMP: ACC:
            break;

        case ABS:
            operands.address = get_address(rom, cpu.pc);
            operands.value = read_mem(operands.address);
            break;

        case ABS_X:
            operands.address = get_address(rom, cpu.pc);
            operands.address += cpu.x_register ;
            operands.value = read_mem(operands.address);
            break;

        case ABS_Y:
            operands.address = get_address(rom, cpu.pc);
            operands.address += cpu.y_register ;
            operands.value = read_mem(operands.address);
            break;

        case IMM:
            operands.value = rom[cpu.pc + 1];
            break;

        case IND:
            operands.address = get_address(rom, cpu.pc);
            operands.address = (uint16_t)read_mem(operands.address) | ((uint16_t)read_mem(operands.address + 1)<< 8);
            break;
        case IND_X:
            operands.address = rom [cpu.pc + 1] + cpu.x_register;
            operands.address = (uint16_t)read_mem(operands.address) | ((uint16_t)read_mem(operands.address + 1)<< 8);
            operands.value = read_mem(operands.address);
            break;

        case IND_Y:
            operands.address = (uint16_t)read_mem(rom[cpu.pc+1]) | ((uint16_t)read_mem(rom[cpu.pc+2])<< 8);
            operands.address += cpu.y_register;
            operands.value = read_mem(operands.address);
            break;
        case REL:
            operands.offset = rom[cpu.pc+1];
            break;
        case ZP:
            operands.value = read_mem(rom[cpu.pc+1]);
            break;
        case ZP_X:
            operands.address = rom[cpu.pc + 1] + cpu.x_register;
            operands.value = read_mem(operands.address);
            break;

        case ZP_Y:
            operands.address = rom[cpu.pc + 1] + cpu.y_register;
            operands.value = read_mem(operands.address);
            break;

        default:
            break;


}
    return operands;
}

void cpu_execute(uint8_t rom [], CPU cpu){

    Operands operands = get_mode_operands(rom, opcodes[cpu.pc].mode, cpu);
    opcodes[cpu.pc].implementation(operands);

}

uint16_t get_address(uint8_t rom [], uint16_t pc){
    return (uint16_t)rom[pc+1] | ((uint16_t)rom[pc+2] << 8);
}
