#include "CPU.h"
#include "bus.h"
#include <stdint.h>

CPU cpu_init(){
    CPU cpu;
    cpu.accumulator = 0;
    cpu.x_register = 0;
    cpu.y_register = 0;
    cpu.pc = RESET_VECTOR;
    cpu.sp = 0xFD;     //NV1BDIZC
    cpu.status_flags = 0b00000100;
    return cpu;
}

void stack_push(CPU *cpu, uint8_t value);
uint8_t stack_pull(CPU *cpu);

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
            operands.address = get_address(cpu->pc+1);
            operands.value = bus_read(operands.address);
            break;

        case ABS_X:
            operands.address = get_address(cpu->pc+1);
            operands.address += cpu->x_register ;
            operands.value = bus_read(operands.address);
            break;

        case ABS_Y:
            operands.address = get_address(cpu->pc+1);
            operands.address += cpu->y_register ;
            operands.value = bus_read(operands.address);
            break;

        case IMM:
            operands.value = bus_read((cpu->pc)+1);
            break;

        case IND:
            operands.address = get_address(cpu->pc+1);
            operands.address = get_address(operands.address);
            break;
        case IND_X:
            operands.address = (uint8_t)(bus_read(cpu->pc+1) + cpu->x_register);
            operands.address = get_address(operands.address);
            operands.value = bus_read(operands.address);
            break;

        case IND_Y:
            operands.address = get_address(cpu->pc+1);
            operands.address += cpu->y_register;
            operands.value = bus_read(operands.address);
            break;
        case REL:
            operands.offset = bus_read((cpu->pc)+1);
            break;

        case ZP:
            operands.address = bus_read((cpu->pc)+1);
            operands.value = bus_read(operands.address);
            break;

        case ZP_X:
            operands.address = (uint8_t)((bus_read((cpu->pc)+1)) + cpu->x_register);
            operands.value = bus_read(operands.address);
            break;

        case ZP_Y:
            operands.address = (uint8_t)((bus_read((cpu->pc)+1)) + cpu->y_register);
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

uint16_t get_address(uint16_t addr){
    return bus_read(addr) | ((bus_read(addr+1)) << 8);
}
uint8_t stack_pull(CPU *cpu){
    cpu->sp = cpu->sp + 0x01;
    return bus_read(cpu->sp+0x0100);


}
void stack_push(CPU *cpu, uint8_t value){
    bus_write((cpu->sp+0x0100), value);
    cpu->sp = cpu->sp - 0x01;
}

void ADC(Operands *oper, CPU *cpu){
    uint16_t result;
    uint8_t accumulator = cpu->accumulator;
    result = cpu->accumulator + oper->value + (cpu->status_flags & FLAG_CARRY);
    cpu->accumulator = result & 0xFF;
    //set carry flag
    cpu->status_flags = (cpu->status_flags &  FLAG_CARRY_CLEAR) | ((result & 0x100) >> 8);
    //set negative flag
    cpu->status_flags = (cpu->status_flags & FLAG_NEGATIVE_CLEAR) | (cpu->accumulator & 0x80);
    //set overflow flag
    cpu->status_flags = (cpu->status_flags & FLAG_OVERFLOW_CLEAR) | (((~(accumulator ^ oper->value) & (accumulator ^ result)) & 0x80) >> 1);
    //set zero flag
    cpu->status_flags = (cpu->status_flags & FLAG_ZERO_CLEAR) | ((1 - ((cpu->accumulator | (~cpu->accumulator + 1)) >> 7))<<1);
}
void AND(Operands *oper, CPU *cpu){
    cpu->accumulator = cpu->accumulator & oper->value;
    //set negative flag
    cpu->status_flags = (cpu->status_flags & FLAG_NEGATIVE_CLEAR) | (cpu->accumulator & 0x80);
    //set zero flag
    cpu->status_flags = (cpu->status_flags & FLAG_ZERO_CLEAR) | ((1 - ((cpu->accumulator | (~cpu->accumulator + 1)) >> 7))<<1);
}
void ASL_ACC(Operands operands, CPU *cpu){
    //set carry flag
    cpu->status_flags = (cpu->status_flags &  FLAG_CARRY_CLEAR) | ((cpu->accumulator & 0x80) >> 7);
    cpu->accumulator = cpu->accumulator << 1;
    //set zero flag
    cpu->status_flags = (cpu->status_flags & FLAG_ZERO_CLEAR) | ((1 - ((cpu->accumulator | (~cpu->accumulator + 1)) >> 7))<<1);
    //set negative flag
    cpu->status_flags = (cpu->status_flags & FLAG_NEGATIVE_CLEAR) | (cpu->accumulator & 0x80);
}
void ASL(Operands *oper, CPU *cpu){
    //set carry flag
    cpu->status_flags = (cpu->status_flags &  FLAG_CARRY_CLEAR) | ((oper->value & 0x80) >> 7);
    oper->value  = oper->value << 1;
    //set zero flag
    cpu->status_flags = (cpu->status_flags & FLAG_ZERO_CLEAR) | ((1 - ((oper->value | (~oper->value + 1)) >> 7))<<1);
    //set negative flag
    cpu->status_flags = (cpu->status_flags & FLAG_NEGATIVE_CLEAR) | (oper->value & 0x80);
    bus_write(oper->address, oper->value);
}
void BCC(Operands *oper, CPU *cpu){
    if(!(cpu->status_flags & FLAG_CARRY)){
        cpu->pc+=oper->offset;
    }
}
void BCS(Operands *oper, CPU *cpu){
    if(cpu->status_flags & FLAG_CARRY){
        cpu->pc+=oper->offset;
    }
}
void BEQ(Operands *oper, CPU *cpu){
    if(cpu->status_flags & FLAG_ZERO){
        cpu->pc+=oper->offset;
    }
}
void BIT(Operands *oper, CPU *cpu){
    uint8_t result;
    //set overflow and negative flag to bit 6 and 7 of value respectively
    cpu->status_flags = (cpu->status_flags & (FLAG_NEGATIVE_CLEAR & FLAG_OVERFLOW_CLEAR)) | (oper->value & 0xC0);
    result = cpu->accumulator & oper->value;
    //set zero flag
    cpu->status_flags = (cpu->status_flags & FLAG_ZERO_CLEAR) | ((1 - ((result | (~result + 1)) >> 7))<<1);

}
void BMI(Operands *oper, CPU *cpu){
    if(cpu->status_flags & FLAG_NEGATIVE){
        cpu->pc+=oper->offset;
    }
}
void BNE(Operands *oper, CPU *cpu){
    if(!(cpu->status_flags & FLAG_ZERO)){
        cpu->pc+=oper->offset;
    }
}
void BPL(Operands *oper, CPU *cpu){
    if(!(cpu->status_flags & FLAG_NEGATIVE)){
        cpu->pc+=oper->offset;
    }
}
void BRK(Operands *oper, CPU *cpu){
    //set break command
    cpu->status_flags = (cpu->status_flags & FLAG_BREAK_CLEAR ) | FLAG_BREAK;
    stack_push(cpu, (cpu->pc >> 8));
    stack_push(cpu, (cpu->pc & 0XFF));
    stack_push(cpu, cpu->status_flags );
    cpu->pc = get_address(IRQ_VECTOR);

}
void BVC(Operands *oper, CPU *cpu){
    if(!(cpu->status_flags & FLAG_OVERFLOW)){
        cpu->pc+=oper->offset;
    }
}
void BVS(Operands *oper, CPU *cpu){
    if(cpu->status_flags & FLAG_OVERFLOW){
        cpu->pc+=oper->offset;
    }
}
void CLC(Operands *oper, CPU *cpu){
    cpu->status_flags = cpu->status_flags & FLAG_CARRY_CLEAR;
}
void CLD(Operands *oper, CPU *cpu){
    cpu->status_flags = cpu->status_flags & FLAG_DECIMAL_CLEAR;
}
void CLI(Operands *oper, CPU *cpu){
    cpu->status_flags = cpu->status_flags & FLAG_INTERRUPT_CLEAR;
}
void CLV(Operands *oper, CPU *cpu){
    cpu->status_flags = cpu->status_flags & FLAG_OVERFLOW_CLEAR;
}

