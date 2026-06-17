#include "CPU.h"
#include "bus.h"
#include <stdint.h>

CPU cpu_init(){
    CPU cpu;
    cpu.accumulator = 0;
    cpu.x_register = 0;
    cpu.y_register = 0;
    cpu.pc = 0xFFFC;
    cpu.sp = 0;        //NV1BDIZC
    cpu.status_flags = 0b00000000;
    return cpu;
}

uint16_t get_address(uint16_t pc);

Operands fetch_operands(Addressin_mode mode, CPU *cpu){
    Operands operands;

    operands.address = 0;
    operands.value = 0;
    operands.offset = 0;


    switch (mode) {

        case IMP:
            break;

        case  ACC:
            break;

        case ABS:
            operands.address = get_address(cpu->pc);
            operands.value = bus_read(operands.address);
            break;

        case ABS_X:
            operands.address = get_address(cpu->pc);
            operands.address += cpu->x_register ;
            operands.value = bus_read(operands.address);
            break;

        case ABS_Y:
            operands.address = get_address(cpu->pc);
            operands.address += cpu->y_register ;
            operands.value = bus_read(operands.address);
            break;

        case IMM:
            operands.value = bus_read((cpu->pc)+1);
            break;

        case IND:
            operands.address = get_address(cpu->pc);
            operands.address = (uint16_t)bus_read(operands.address) | ((uint16_t)bus_read(operands.address + 1)<< 8);
            break;
        case IND_X:
            operands.address = (bus_read((cpu->pc)+1)) + cpu->x_register | ((bus_read((cpu->pc)+2)) + cpu->x_register)<< 8;
            operands.address = (uint16_t)bus_read(operands.address) | ((uint16_t)bus_read(operands.address + 1)<< 8);
            operands.value = bus_read(operands.address);
            break;

        case IND_Y:
            operands.address = (uint16_t)bus_read((cpu->pc)+1) | ((uint16_t)bus_read((cpu->pc)+2)<< 8);
            operands.address += cpu->y_register;
            operands.value = bus_read(operands.address);
            break;
        case REL:
            operands.offset = bus_read((cpu->pc)+1);
            break;

        case ZP:
            operands.value = bus_read((cpu->pc)+1);
            break;

        case ZP_X:
            operands.address = (bus_read((cpu->pc)+1)) + cpu->x_register;
            operands.value = bus_read(operands.address);
            break;

        case ZP_Y:
            operands.address = (bus_read((cpu->pc)+1)) + cpu->y_register;
            operands.value = bus_read(operands.address);
            break;

        default:
            break;


}
    return operands;
}

void cpu_execute(CPU *cpu){

    Operands operands = fetch_operands(opcodes[cpu->pc].mode, cpu);
    opcodes[cpu->pc].implementation(&operands, cpu);

}

uint16_t get_address(uint16_t pc){
    return bus_read(pc+1) | ((bus_read(pc+2)) << 8);
}

void ADC(Operands *oper, CPU *cpu){
    uint16_t result;
    uint8_t accumulator = cpu->accumulator;
    result = cpu->accumulator + oper->value + (cpu->status_flags & 0x01);
    cpu->accumulator = result & 0xFF;

    //set carry flag
    cpu->status_flags = (cpu->status_flags &  0xFE) | ((result & 0x100) >> 8);
    //set negative flag
    cpu->status_flags = (cpu->status_flags & 0X7F) | (cpu->accumulator & 0x80);
    //set overflow flag
    cpu->status_flags = (cpu->status_flags & 0XBF) | (((~(accumulator ^ oper->value) & (accumulator ^ result)) & 0x80) >> 1);
    //set zero flag
    cpu->status_flags = (cpu->status_flags & 0XFD) | ((1 - ((cpu->accumulator | (~cpu->accumulator + 1)) >> 7))<<1);
}
void AND(Operands *oper, CPU *cpu){
    cpu->accumulator = cpu->accumulator & oper->value;
    //set negative flag
    cpu->status_flags = (cpu->status_flags & 0X7F) | (cpu->accumulator & 0x80);
    //set zero flag
    cpu->status_flags = (cpu->status_flags & 0XFD) | ((1 - ((cpu->accumulator | (~cpu->accumulator + 1)) >> 7))<<1);
}
void ALS_ACC(Operands operands, CPU *cpu){
    //set carry flag
    cpu->status_flags = (cpu->status_flags &  0xFE) | ((cpu->accumulator & 0x80) >> 7);
    cpu->accumulator = cpu->accumulator << 1;
    //set zero flag
    cpu->status_flags = (cpu->status_flags & 0XFD) | ((1 - ((cpu->accumulator | (~cpu->accumulator + 1)) >> 7))<<1);
}
void ALS(Operands *oper, CPU *cpu){
    //set carry flag
    cpu->status_flags = (cpu->status_flags &  0xFE) | ((oper->value & 0x80) >> 7);
    oper->value  = oper->value << 1;
    //set zero flag
    cpu->status_flags = (cpu->status_flags & 0XFD) | ((1 - ((cpu->accumulator | (~cpu->accumulator + 1)) >> 7))<<1);
}
void BCC(Operands *oper, CPU *cpu){
    if(cpu->status_flags & 1){
        cpu->pc=+oper->offset;
    }
}
