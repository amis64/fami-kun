#include "CPU.h"
#include "bus.h"
#include <stdint.h>

static void stack_push(CPU *cpu, uint8_t value);
static uint8_t stack_pull(CPU *cpu);
static uint16_t get_address(uint16_t pc);
static void set_negative_flag(CPU *cpu, uint8_t value);
static void set_zero_flag(CPU *cpu, uint8_t value);
void ADC(Operands *oper, CPU *cpu);
void AND(Operands *oper, CPU *cpu);
void ASL_ACC(Operands *oper, CPU *cpu);
void ASL(Operands *oper, CPU *cpu);
void BCC(Operands *oper, CPU *cpu);
void BCS(Operands *oper, CPU *cpu);
void BEQ(Operands *oper, CPU *cpu);
void BIT(Operands *oper, CPU *cpu);
void BMI(Operands *oper, CPU *cpu);
void BNE(Operands *oper, CPU *cpu);
void BPL(Operands *oper, CPU *cpu);
void BRK(Operands *oper, CPU *cpu);
void BVC(Operands *oper, CPU *cpu);
void BVS(Operands *oper, CPU *cpu);
void CLC(Operands *oper, CPU *cpu);
void CLD(Operands *oper, CPU *cpu);
void CLI(Operands *oper, CPU *cpu);
void CLV(Operands *oper, CPU *cpu);
void CMP(Operands *oper, CPU *cpu);
void CPX(Operands *oper, CPU *cpu);
void CPY(Operands *oper, CPU *cpu);
void DEC(Operands *oper, CPU *cpu);
void DEX(Operands *oper, CPU *cpu);
void DEY(Operands *oper, CPU *cpu);
void EOR(Operands *oper, CPU *cpu);
void INC(Operands *oper, CPU *cpu);
void INX(Operands *oper, CPU *cpu);
void INY(Operands *oper, CPU *cpu);
void JMP(Operands *oper, CPU *cpu);
void JSR(Operands *oper, CPU *cpu);
void LDA(Operands *oper, CPU *cpu);
void LDX(Operands *oper, CPU *cpu);
void LDY(Operands *oper, CPU *cpu);
void LSR_ACC(Operands *oper, CPU *cpu);
void LSR(Operands *oper, CPU *cpu);
void NOP(Operands *oper, CPU *cpu);
void ORA(Operands *oper, CPU *cpu);
void PHA(Operands *oper, CPU *cpu);
void PHP(Operands *oper, CPU *cpu);
void PLA(Operands *oper, CPU *cpu);
void PLP(Operands *oper, CPU *cpu);
void ROL_ACC(Operands *oper, CPU *cpu);
void ROL(Operands *oper, CPU *cpu);
void ROR_ACC(Operands *oper, CPU *cpu);
void ROR(Operands *oper, CPU *cpu);
void RTI(Operands *oper, CPU *cpu);
void RTS(Operands *oper, CPU *cpu);
void SBC(Operands *oper, CPU *cpu);
void SEC(Operands *oper, CPU *cpu);
void SED(Operands *oper, CPU *cpu);
void SEI(Operands *oper, CPU *cpu);
void STA(Operands *oper, CPU *cpu);
void STX(Operands *oper, CPU *cpu);
void STY(Operands *oper, CPU *cpu);
void TAX(Operands *oper, CPU *cpu);
void TAY(Operands *oper, CPU *cpu);
void TSX(Operands *oper, CPU *cpu);
void TXA(Operands *oper, CPU *cpu);
void TXS(Operands *oper, CPU *cpu);
void TYA(Operands *oper, CPU *cpu);

const Instruction opcodes[OPCODE_TABLE_SIZE] = {
    // 0x00
    {BRK,IMP,7,0},{ORA,IND_X,6,0},{NOP,IMP,1,0},{NOP,IND_X,8,0},{NOP,ZP,3,0},{ORA,ZP,3,0},{ASL,ZP,5,0},{NOP,ZP,5,0},{PHP,IMP,3,0},{ORA,IMM,2,0},{ASL_ACC,ACC,2,0},{NOP,IMM,2,0},{NOP,ABS,4,0},{ORA,ABS,4,0},{ASL,ABS,6,0},{NOP,ABS,6,0},
    // 0x10
    {BPL,REL,2,0},{ORA,IND_Y,5,1},{NOP,IMP,1,0},{NOP,IND_Y,8,0},{NOP,ZP_X,4,0},{ORA,ZP_X,4,0},{ASL,ZP_X,6,0},{NOP,ZP_X,6,0},{CLC,IMP,2,0},{ORA,ABS_Y,4,1},{NOP,IMP,2,0},{NOP,ABS_Y,7,0},{NOP,ABS_X,4,1},{ORA,ABS_X,4,1},{ASL,ABS_X,7,0},{NOP,ABS_X,7,0},
    // 0x20
    {JSR,ABS,6,0},{AND,IND_X,6,0},{NOP,IMP,1,0},{NOP,IND_X,8,0},{BIT,ZP,3,0},{AND,ZP,3,0},{ROL,ZP,5,0},{NOP,ZP,5,0},{PLP,IMP,4,0},{AND,IMM,2,0},{ROL_ACC,ACC,2,0},{NOP,IMM,2,0},{BIT,ABS,4,0},{AND,ABS,4,0},{ROL,ABS,6,0},{NOP,ABS,6,0},
    // 0x30
    {BMI,REL,2,0},{AND,IND_Y,5,1},{NOP,IMP,1,0},{NOP,IND_Y,8,0},{NOP,ZP_X,4,0},{AND,ZP_X,4,0},{ROL,ZP_X,6,0},{NOP,ZP_X,6,0},{SEC,IMP,2,0},{AND,ABS_Y,4,1},{NOP,IMP,2,0},{NOP,ABS_Y,7,0},{NOP,ABS_X,4,1},{AND,ABS_X,4,1},{ROL,ABS_X,7,0},{NOP,ABS_X,7,0},
    // 0x40
    {RTI,IMP,6,0},{EOR,IND_X,6,0},{NOP,IMP,1,0},{NOP,IND_X,8,0},{NOP,ZP,3,0},{EOR,ZP,3,0},{LSR,ZP,5,0},{NOP,ZP,5,0},{PHA,IMP,3,0},{EOR,IMM,2,0},{LSR_ACC,ACC,2,0},{NOP,IMM,2,0},{JMP,ABS,3,0},{EOR,ABS,4,0},{LSR,ABS,6,0},{NOP,ABS,6,0},
    // 0x50
    {BVC,REL,2,0},{EOR,IND_Y,5,1},{NOP,IMP,1,0},{NOP,IND_Y,8,0},{NOP,ZP_X,4,0},{EOR,ZP_X,4,0},{LSR,ZP_X,6,0},{NOP,ZP_X,6,0},{CLI,IMP,2,0},{EOR,ABS_Y,4,1},{NOP,IMP,2,0},{NOP,ABS_Y,7,0},{NOP,ABS_X,4,1},{EOR,ABS_X,4,1},{LSR,ABS_X,7,0},{NOP,ABS_X,7,0},
    // 0x60
    {RTS,IMP,6,0},{ADC,IND_X,6,0},{NOP,IMP,1,0},{NOP,IND_X,8,0},{NOP,ZP,3,0},{ADC,ZP,3,0},{ROR,ZP,5,0},{NOP,ZP,5,0},{PLA,IMP,4,0},{ADC,IMM,2,0},{ROR_ACC,ACC,2,0},{NOP,IMM,2,0},{JMP,IND,5,0},{ADC,ABS,4,0},{ROR,ABS,6,0},{NOP,ABS,6,0},
    // 0x70
    {BVS,REL,2,0},{ADC,IND_Y,5,1},{NOP,IMP,1,0},{NOP,IND_Y,8,0},{NOP,ZP_X,4,0},{ADC,ZP_X,4,0},{ROR,ZP_X,6,0},{NOP,ZP_X,6,0},{SEI,IMP,2,0},{ADC,ABS_Y,4,1},{NOP,IMP,2,0},{NOP,ABS_Y,7,0},{NOP,ABS_X,4,1},{ADC,ABS_X,4,1},{ROR,ABS_X,7,0},{NOP,ABS_X,7,0},
    // 0x80
    {NOP,IMM,2,0},{STA,IND_X,6,0},{NOP,IMM,2,0},{NOP,IND_X,6,0},{STY,ZP,3,0},{STA,ZP,3,0},{STX,ZP,3,0},{NOP,ZP,3,0},{DEY,IMP,2,0},{NOP,IMM,2,0},{TXA,IMP,2,0},{NOP,IMM,2,0},{STY,ABS,4,0},{STA,ABS,4,0},{STX,ABS,4,0},{NOP,ABS,4,0},
    // 0x90
    {BCC,REL,2,0},{STA,IND_Y,6,0},{NOP,IMP,2,0},{NOP,IND_Y,6,0},{STY,ZP_X,4,0},{STA,ZP_X,4,0},{STX,ZP_Y,4,0},{NOP,ZP_Y,4,0},{TYA,IMP,2,0},{STA,ABS_Y,5,0},{TXS,IMP,2,0},{NOP,ABS_Y,5,0},{NOP,ABS_X,5,0},{STA,ABS_X,5,0},{NOP,ABS_Y,5,0},{NOP,ABS_Y,5,0},
    // 0xA0
    {LDY,IMM,2,0},{LDA,IND_X,6,0},{LDX,IMM,2,0},{NOP,IND_X,6,0},{LDY,ZP,3,0},{LDA,ZP,3,0},{LDX,ZP,3,0},{NOP,ZP,3,0},{TAY,IMP,2,0},{LDA,IMM,2,0},{TAX,IMP,2,0},{NOP,IMM,2,0},{LDY,ABS,4,0},{LDA,ABS,4,0},{LDX,ABS,4,0},{NOP,ABS,4,0},
    // 0xB0
    {BCS,REL,2,0},{LDA,IND_Y,5,1},{NOP,IMP,2,0},{NOP,IND_Y,5,1},{LDY,ZP_X,4,0},{LDA,ZP_X,4,0},{LDX,ZP_Y,4,0},{NOP,ZP_Y,4,0},{CLV,IMP,2,0},{LDA,ABS_Y,4,1},{TSX,IMP,2,0},{NOP,ABS_Y,4,0},{LDY,ABS_X,4,1},{LDA,ABS_X,4,1},{LDX,ABS_Y,4,1},{NOP,ABS_Y,4,0},
    // 0xC0
    {CPY,IMM,2,0},{CMP,IND_X,6,0},{NOP,IMM,2,0},{NOP,IND_X,8,0},{CPY,ZP,3,0},{CMP,ZP,3,0},{DEC,ZP,5,0},{NOP,ZP,5,0},{INY,IMP,2,0},{CMP,IMM,2,0},{DEX,IMP,2,0},{NOP,IMM,2,0},{CPY,ABS,4,0},{CMP,ABS,4,0},{DEC,ABS,6,0},{NOP,ABS,6,0},
    // 0xD0
    {BNE,REL,2,0},{CMP,IND_Y,5,1},{NOP,IMP,2,0},{NOP,IND_Y,8,0},{NOP,ZP_X,4,0},{CMP,ZP_X,4,0},{DEC,ZP_X,6,0},{NOP,ZP_X,6,0},{CLD,IMP,2,0},{CMP,ABS_Y,4,1},{NOP,IMP,2,0},{NOP,ABS_Y,7,0},{NOP,ABS_X,4,1},{CMP,ABS_X,4,1},{DEC,ABS_X,7,0},{NOP,ABS_X,7,0},
    // 0xE0
    {CPX,IMM,2,0},{SBC,IND_X,6,0},{NOP,IMM,2,0},{NOP,IND_X,8,0},{CPX,ZP,3,0},{SBC,ZP,3,0},{INC,ZP,5,0},{NOP,ZP,5,0},{INX,IMP,2,0},{SBC,IMM,2,0},{NOP,IMP,2,0},{SBC,IMM,2,0},{CPX,ABS,4,0},{SBC,ABS,4,0},{INC,ABS,6,0},{NOP,ABS,6,0},
    // 0xF0
    {BEQ,REL,2,0},{SBC,IND_Y,5,1},{NOP,IMP,2,0},{NOP,IND_Y,8,0},{NOP,ZP_X,4,0},{SBC,ZP_X,4,0},{INC,ZP_X,6,0},{NOP,ZP_X,6,0},{SED,IMP,2,0},{SBC,ABS_Y,4,1},{NOP,IMP,2,0},{NOP,ABS_Y,7,0},{NOP,ABS_X,4,1},{SBC,ABS_X,4,1},{INC,ABS_X,7,0},{NOP,ABS_X,7,0},
};
CPU cpu_init(){
    CPU cpu;
    cpu.accumulator = 0;
    cpu.x_register = 0;
    cpu.y_register = 0;
    cpu.pc = 0xC000;
    cpu.sp = 0xFD;     //NV1BDIZC
    cpu.status_flags = 0x24;
    return cpu;
}

int cpu_execute(CPU *cpu){
    uint8_t extra_cycles = 0;
    uint16_t prev_pc;
    uint8_t opcode = bus_read(cpu->pc);
    cpu->pc++;
    Operands operands = fetch_operands(opcodes[opcode].mode, cpu, &extra_cycles);
    prev_pc = cpu->pc;
    extra_cycles = extra_cycles * opcodes[opcode].page_cross_penalty;
    opcodes[opcode].implementation(&operands, cpu);
    if(opcodes[opcode].mode == REL && prev_pc != cpu->pc){
        extra_cycles += 1; extra_cycles += ((cpu->pc >> 8) != (prev_pc >> 8));
    }
    return opcodes[opcode].cycles + extra_cycles;
}
Operands fetch_operands(Addressin_mode mode, CPU *cpu, uint8_t *extra_cycles){
    Operands operands;
    uint16_t base_address;
    uint8_t low_byte;
    uint16_t high_byte;
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
            cpu->pc += 2;
            break;

        case ABS_X:
            base_address = get_address(cpu->pc);
            operands.address = cpu->x_register + base_address;
            operands.value = bus_read(operands.address);
            *extra_cycles += ((operands.address >> 8) != (base_address >> 8));
            cpu->pc += 2;
            break;

        case ABS_Y:
            base_address = get_address(cpu->pc);
            operands.address = cpu->y_register + base_address;
            operands.value = bus_read(operands.address);
            *extra_cycles += ((operands.address >> 8) != (base_address >> 8));
            cpu->pc += 2;
            break;

        case IMM:
            operands.value = bus_read(cpu->pc);
            cpu->pc++;
            break;

        case IND:
            low_byte = bus_read(cpu->pc);
            high_byte = bus_read(cpu->pc + 1);
            operands.address = bus_read((high_byte << 8 | low_byte)) | (bus_read((high_byte << 8 | (uint8_t)(low_byte + 1))) << 8);
            cpu->pc += 2;
            break;
        case IND_X:
            operands.address = (uint8_t)(bus_read(cpu->pc) + cpu->x_register);
            operands.address = get_address(operands.address);
            operands.value = bus_read(operands.address);
            cpu->pc++;
            break;

        case IND_Y:
            base_address = bus_read(cpu->pc);
            base_address = get_address(base_address);
            operands.address = cpu->y_register + base_address;
            operands.value = bus_read(operands.address);
            *extra_cycles += ((operands.address >> 8) != (base_address >> 8));
            cpu->pc++;
            break;

        case REL:
            operands.offset = bus_read(cpu->pc);
            cpu->pc++;
            break;

        case ZP:
            operands.address = bus_read(cpu->pc);
            operands.value = bus_read(operands.address);
            cpu->pc++;
            break;

        case ZP_X:
            operands.address = (uint8_t)(bus_read(cpu->pc) + cpu->x_register);
            operands.value = bus_read(operands.address);
            cpu->pc++;
            break;

        case ZP_Y:
            operands.address = (uint8_t)(bus_read(cpu->pc) + cpu->y_register);
            operands.value = bus_read(operands.address);
            cpu->pc++;
            break;

        default:
            break;
}
    return operands;
}

void ADC(Operands *oper, CPU *cpu){
    uint16_t result;
    uint8_t accumulator = cpu->accumulator;
    result = cpu->accumulator + oper->value + (cpu->status_flags & FLAG_CARRY);
    cpu->accumulator = result & 0xFF;
    //set carry flag
    cpu->status_flags = (cpu->status_flags &  FLAG_CARRY_CLEAR) | ((result & 0x100) >> 8);
    //set overflow flag
    cpu->status_flags = (cpu->status_flags & FLAG_OVERFLOW_CLEAR) | (((~(accumulator ^ oper->value) & (accumulator ^ result)) & 0x80) >> 1);
    //set zero flag
    set_zero_flag(cpu, cpu->accumulator);
    //set negative flag
    set_negative_flag(cpu, cpu->accumulator);
}
void AND(Operands *oper, CPU *cpu){
    cpu->accumulator = cpu->accumulator & oper->value;
    //set zero flag
    set_zero_flag(cpu, cpu->accumulator);
    //set negative flag
    set_negative_flag(cpu, cpu->accumulator);
}
void ASL_ACC(Operands *oper, CPU *cpu){
    //set carry flag
    cpu->status_flags = (cpu->status_flags &  FLAG_CARRY_CLEAR) | ((cpu->accumulator & 0x80) >> 7);
    cpu->accumulator = cpu->accumulator << 1;
    //set zero flag
    set_zero_flag(cpu, cpu->accumulator);
    //set negative flag
    set_negative_flag(cpu, cpu->accumulator);
}
void ASL(Operands *oper, CPU *cpu){
    //set carry flag
    cpu->status_flags = (cpu->status_flags &  FLAG_CARRY_CLEAR) | ((oper->value & 0x80) >> 7);
    oper->value  = oper->value << 1;
    //set zero flag
    set_zero_flag(cpu, oper->value);
    //set negative flag
    set_negative_flag(cpu, oper->value);
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
    set_zero_flag(cpu, result);
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
void CMP(Operands *oper, CPU *cpu){
    uint16_t result;
    result = cpu->accumulator - oper->value;
    //set carry flag
    cpu->status_flags = (cpu->status_flags & FLAG_CARRY_CLEAR) | (~(result >> 0x08) & FLAG_CARRY);
    //set zero flag
    set_zero_flag(cpu, (uint8_t)result);
    //set negative flag
    set_negative_flag(cpu, result);

}
void CPX(Operands *oper, CPU *cpu){
    uint16_t result;
    result = cpu->x_register - oper->value;
    //set carry flag
    cpu->status_flags = (cpu->status_flags & FLAG_CARRY_CLEAR) | (~(result >> 0x08) & FLAG_CARRY);
    //set zero flag
    set_zero_flag(cpu, (uint8_t)result);
    //set negative flag
    set_negative_flag(cpu, result);
}
void CPY(Operands *oper, CPU *cpu){
    uint16_t result;
    result = cpu->y_register - oper->value;
    //set carry flag
    cpu->status_flags = (cpu->status_flags & FLAG_CARRY_CLEAR) | (~(result >> 0x08) & FLAG_CARRY);
    //set zero flag
    set_zero_flag(cpu, (uint8_t)result);
    //set negative flag
    set_negative_flag(cpu, result);
}
void DEC(Operands *oper, CPU *cpu){
    oper->value -= 0x01;
    //set zero flag
    set_zero_flag(cpu, oper->value);
    //set negative flag
    set_negative_flag(cpu, oper->value);
    bus_write(oper->address, oper->value);
}
void DEX(Operands *oper, CPU *cpu){
    cpu->x_register -= 0x01;
    //set zero flag
    set_zero_flag(cpu, cpu->x_register);
    //set negative flag
    set_negative_flag(cpu, cpu->x_register);
}
void DEY(Operands *oper, CPU *cpu){
    cpu->y_register -= 0x01;
    //set zero flag
    set_zero_flag(cpu, cpu->y_register);
    //set negative flag
    set_negative_flag(cpu, cpu->y_register);
}
void EOR(Operands *oper, CPU *cpu){
    cpu->accumulator = cpu->accumulator ^ oper->value;
    //set zero flag
    set_zero_flag(cpu, cpu->accumulator);
    //set negative flag
    set_negative_flag(cpu, cpu->accumulator);
}
void INC(Operands *oper, CPU *cpu){
    oper->value += 0x01;
    //set zero flag
    set_zero_flag(cpu, oper->value);
    //set negative flag
    set_negative_flag(cpu, oper->value);
    bus_write(oper->address, oper->value);
}
void INX(Operands *oper, CPU *cpu){
    cpu->x_register += 0x01;
    //set zero flag
    set_zero_flag(cpu, cpu->x_register);
    //set negative flag
    set_negative_flag(cpu, cpu->x_register);
}
void INY(Operands *oper, CPU *cpu){
    cpu->y_register += 0x01;
    //set zero flag
    set_zero_flag(cpu, cpu->y_register);
    //set negative flag
    set_negative_flag(cpu, cpu->y_register);
}
void JMP(Operands *oper, CPU *cpu){
    cpu->pc = oper->address;
}
void JSR(Operands *oper, CPU *cpu){
    stack_push(cpu, ((cpu->pc-1) >> 8));
    stack_push(cpu, ((cpu->pc-1) & 0XFF));
    cpu->pc = oper->address;
}
void LDA(Operands *oper, CPU *cpu){
    cpu->accumulator = oper->value;
    //set zero flag
    set_zero_flag(cpu, cpu->accumulator);
    //set negative flag
    set_negative_flag(cpu, cpu->accumulator);
}
void LDX(Operands *oper, CPU *cpu){
    cpu->x_register = oper->value;
    //set zero flag
    set_zero_flag(cpu, cpu->x_register);
    //set negative flag
    set_negative_flag(cpu, cpu->x_register);
}
void LDY(Operands *oper, CPU *cpu){
    cpu->y_register = oper->value;
    //set zero flag
    set_zero_flag(cpu, cpu->y_register);
    //set negative flag
    set_negative_flag(cpu, cpu->y_register);
}
void LSR_ACC(Operands *oper, CPU *cpu){
    //set carry flag
    cpu->status_flags = (cpu->status_flags &  FLAG_CARRY_CLEAR) | (cpu->accumulator & 0x01);
    cpu->accumulator = cpu->accumulator >> 1;
    //set zero flag
    set_zero_flag(cpu, cpu->accumulator);
    //set negative flag
    set_negative_flag(cpu, cpu->accumulator);
}
void LSR(Operands *oper, CPU *cpu){
    //set carry flag
    cpu->status_flags = (cpu->status_flags &  FLAG_CARRY_CLEAR) | (oper->value & 0x01);
    oper->value  = oper->value >> 1;
    //set zero flag
    set_zero_flag(cpu, oper->value);
    //set negative flag
    set_negative_flag(cpu, oper->value);
    bus_write(oper->address, oper->value);
}
void NOP(Operands *oper, CPU *cpu){}
void ORA(Operands *oper, CPU *cpu){
    cpu->accumulator = cpu->accumulator | oper->value;
    //set zero flag
    set_zero_flag(cpu, cpu->accumulator);
    //set negative flag
    set_negative_flag(cpu, cpu->accumulator);
}
void PHA(Operands *oper, CPU *cpu){
    stack_push(cpu, cpu->accumulator);
}
void PHP(Operands *oper, CPU *cpu){
    stack_push(cpu, cpu->status_flags| 0x10 | 0x20);
}
void PLA(Operands *oper, CPU *cpu){
    cpu->accumulator = stack_pull(cpu);
    //set zero flag
    set_zero_flag(cpu, cpu->accumulator);
    //set negative flag
    set_negative_flag(cpu, cpu->accumulator);
}
void PLP(Operands *oper, CPU *cpu){
    cpu->status_flags = stack_pull(cpu);
    cpu->status_flags = (cpu->status_flags & ~0x10) | 0x20;
}
void ROL_ACC(Operands *oper, CPU *cpu){

    uint8_t bit_7 = (cpu->accumulator & 0x80) >>0x07;
    cpu->accumulator = (cpu->accumulator << 1) | (cpu->status_flags & FLAG_CARRY);
    cpu->status_flags = (cpu->status_flags & FLAG_CARRY_CLEAR) | bit_7;
    //set zero flag
    set_zero_flag(cpu, cpu->accumulator);
    //set negative flag
    set_negative_flag(cpu, cpu->accumulator);
}
void ROL(Operands *oper, CPU *cpu){
    uint8_t bit_7 = (oper->value & 0x80) >> 0x07;
    oper->value = (oper->value << 1) | (cpu->status_flags & FLAG_CARRY);
    cpu->status_flags = (cpu->status_flags & FLAG_CARRY_CLEAR) | bit_7;
    //set zero flag
    set_zero_flag(cpu, oper->value);
    //set negative flag
    set_negative_flag(cpu, oper->value);
    bus_write(oper->address, oper->value);
}
void ROR_ACC(Operands *oper, CPU *cpu){

    uint8_t bit_0 = (cpu->accumulator & 0x01);
    cpu->accumulator = (cpu->accumulator >> 1) | ((cpu->status_flags & FLAG_CARRY) << 0x07);
    cpu->status_flags = (cpu->status_flags & FLAG_CARRY_CLEAR) | bit_0;
    //set zero flag
    set_zero_flag(cpu, cpu->accumulator);
    //set negative flag
    set_negative_flag(cpu, cpu->accumulator);
}
void ROR(Operands *oper, CPU *cpu){

    uint8_t bit_0 = (oper->value & 0x01);
    oper->value = (oper->value >> 1) | ((cpu->status_flags & FLAG_CARRY) << 0x07);
    cpu->status_flags = (cpu->status_flags & FLAG_CARRY_CLEAR) | bit_0;
    //set zero flag
    set_zero_flag(cpu, cpu->accumulator);
    //set negative flag
    set_negative_flag(cpu, oper->value);
    bus_write(oper->address, oper->value);
}
void RTI(Operands *oper, CPU *cpu){
    cpu->status_flags = stack_pull(cpu);
    uint16_t first_pull = stack_pull(cpu);
    cpu->pc = first_pull | (stack_pull(cpu) << 8);
    cpu->status_flags = (cpu->status_flags & ~0x10) | 0x20;
}
void RTS(Operands *oper, CPU *cpu){
    uint16_t first_pull = stack_pull(cpu);
    cpu->pc = first_pull | (stack_pull(cpu) << 8);
    cpu->pc++;
}
void SBC(Operands *oper, CPU *cpu){
    oper->value = ~oper->value;
    ADC(oper, cpu);
}
void SEC(Operands *oper, CPU *cpu){
    cpu->status_flags = cpu->status_flags | FLAG_CARRY;
}
void SED(Operands *oper, CPU *cpu){
    cpu->status_flags = cpu->status_flags | FLAG_DECIMAL;
}
void SEI(Operands *oper, CPU *cpu){
    cpu->status_flags = cpu->status_flags | FLAG_INTERRUPT;
}
void STA(Operands *oper, CPU *cpu){
    bus_write(oper->address, cpu->accumulator);
}
void STX(Operands *oper, CPU *cpu){
    bus_write(oper->address, cpu->x_register);
}
void STY(Operands *oper, CPU *cpu){
    bus_write(oper->address, cpu->y_register);
}
void TAX(Operands *oper, CPU *cpu){
    cpu->x_register = cpu->accumulator;
    set_zero_flag(cpu, cpu->x_register);
    set_negative_flag(cpu, cpu->x_register);
}
void TAY(Operands *oper, CPU *cpu){
    cpu->y_register = cpu->accumulator;
    set_zero_flag(cpu, cpu->y_register);
    set_negative_flag(cpu, cpu->y_register);
}

void TSX(Operands *oper, CPU *cpu){
    cpu->x_register = cpu->sp;
    set_zero_flag(cpu, cpu->x_register);
    set_negative_flag(cpu, cpu->x_register);
}
void TXA(Operands *oper, CPU *cpu){
    cpu->accumulator = cpu->x_register;
    set_zero_flag(cpu, cpu->accumulator);
    set_negative_flag(cpu, cpu->accumulator);
}
void TXS(Operands *oper, CPU *cpu){
    cpu->sp = cpu->x_register;
}
void TYA(Operands *oper, CPU *cpu){
    cpu->accumulator = cpu->y_register;
    set_zero_flag(cpu, cpu->accumulator);
    set_negative_flag(cpu, cpu->accumulator);
}

//CPU HELPERS
static uint16_t get_address(uint16_t addr){
    uint16_t next_addr;
    if(addr == 0xFF){
        next_addr = 0x00;
    }else{next_addr = addr + 1;}
    return bus_read(addr) | (bus_read(next_addr) << 8);
}
static uint8_t stack_pull(CPU *cpu){
    cpu->sp = cpu->sp + 0x01;
    return bus_read(cpu->sp+0x0100);
}
static void stack_push(CPU *cpu, uint8_t value){
    bus_write((cpu->sp+0x0100), value);
    cpu->sp = cpu->sp - 0x01;
}
static void set_negative_flag(CPU *cpu, uint8_t value){
    cpu->status_flags = (cpu->status_flags & FLAG_NEGATIVE_CLEAR) | (value & FLAG_NEGATIVE);
}
static void set_zero_flag(CPU *cpu, uint8_t value){
    cpu->status_flags = (cpu->status_flags & FLAG_ZERO_CLEAR) | ((1 - ((value | (~value + 1)) >> 7))<<1);
}
