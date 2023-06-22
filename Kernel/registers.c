#include <registers.h>
//ILLEGAL
static uint64_t registers[17] = {0};
extern uint64_t regs[3];

void showRegisterStatus(){
    enter();
    saveRegisterStatus();
    printRegisterStatus();
    enter();
}

void saveRegisterStatus(){
    registers[0] = getCurrentRAX();
    registers[1] = getCurrentRBX();
    registers[2] = getCurrentRCX();
    registers[3] = getCurrentRDX();
    registers[4] = regs[0];
    registers[5] = getCurrentRSI();
    registers[6] = getCurrentRDI();
    registers[7] = getCurrentR15();
    registers[8] = getCurrentR14();
    registers[9] = getCurrentR13();
    registers[10] = getCurrentR12();
    registers[11] = getCurrentR11();
    registers[12] = getCurrentR10();
    registers[13] = getCurrentR9();
    registers[14] = getCurrentR8();
    registers[15] = regs[2];
    registers[16] = regs[1];
}

void printRegisterStatus(){
    printRegister("A: ", registers[0]);
    printRegister("B: ", registers[1]);
    printRegister("C: ", registers[2]);
    printRegister("D: ", registers[3]);
    printRegister("BP: ", registers[4]);
    printRegister("SI: ", registers[5]);
    printRegister("SP: ", registers[15]);
    printRegister("DI: ", registers[6]);
    printRegister("RIP: ", registers[16]);
    printRegister("15: ", registers[7]);
    printRegister("14: ", registers[8]);
    printRegister("13: ", registers[9]);
    printRegister("12: ", registers[10]);
    printRegister("11: ", registers[11]);
    printRegister("10: ", registers[12]);
    printRegister("9: ", registers[13]);
    printRegister("8: ", registers[14]);
}