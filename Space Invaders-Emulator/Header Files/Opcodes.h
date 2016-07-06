#pragma once
//#include "stdafx.h"
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <SDL.h>

typedef struct ConditionCodes {
	uint8_t    z : 1;
	uint8_t    s : 1;
	uint8_t    p : 1;
	uint8_t    cy : 1;
	uint8_t    ac : 1;
	uint8_t    pad : 3;
} ConditionCodes;



typedef struct State8080 {
	uint8_t    a;
	uint8_t    b;
	uint8_t    c;
	uint8_t    d;
	uint8_t    e;
	uint8_t    h;
	uint8_t    l;
	uint16_t    sp;
	uint16_t    pc;
	uint8_t     *memory;
	struct      ConditionCodes      cc;
	uint8_t     int_enable;
}State8080;
State8080* Init8080(void);
/*UTILITY FUNCTIONS*/

/* Provides a printout for each 8080 opcode*/
int Dissassemble(unsigned char *codebuffer, int pc);
/* Prints the current status of the emulator*/
void PrintStatus(State8080* state);
/*Reads the file with an optional offset in memory*/
void ReadFileIntoMemoryAt(State8080* state, char* filename, uint32_t offset);
/*Outputs the parity of a given integer a, outputting 1 for even, 0 for odd*/
uint8_t Parity(uint8_t a);
/*Used if the program runs into an unimplemented instruction*/
void UnimplementedInstruction(State8080* state);

/*ARITHMETIC GROUP*/
/*
Adds the current state in register A, and the variable value (no carry)
and stores in register A

@param state   state register
@param var     the variable being added to the state

*/
void ADD(State8080 *state, uint8_t var);
/*
Adds the current state in register A and the variable value (with carry)
and stores in register A

@param state   state register
@param var     the variable being added to the state

*/
void ADC(State8080 *state, uint8_t var);
/*
Subtracts the current state in register A and the variable value (no carry)
and stores in register A

@param state   state register
@param var     the variable being added to the state

*/
void SUB(State8080 *state, uint8_t var);
/*
Subtracts the current state in register A and the variable value (with carry)
and stores in register A

@param state   state register
@param var     the variable being added to the state

*/
void SBB(State8080 *state, uint8_t var);
/*
Add 1 to the register pair
Adds to the physical register
(not the address stored within the register pair!!!)
@param reg1 the first
*/
void INX(uint8_t *reg1, uint8_t *reg2);
/*
Subtract 1 from the register
@param reg the incoming register value
*/
void DCX(uint8_t *reg1, uint8_t *reg2);
/*
Performs a 'double add'
the 16 bit number in register pair added to that held in the HL registers
stored in HL registers

@param state provides access to the state variable
@param regP the register pair value to be added
*/
void DAD(State8080 *state, uint32_t regP);
/*
Increments the specified register or memory byte by 1

@param state provides access to the state variable
@param var address of the register to be incremented
*/
void INR(State8080 *state, uint8_t* var);
/*
Decrements the specified register or memory byte by 1

@param state provides access to the state variable
@param var address of the register to be decremented
*/
void DCR(State8080 *state, uint8_t* var);

void Flags(State8080 *state, uint16_t res);
/*BRANCH GROUP*/

/*
jump to the specified values stored in the added
return address is pushed onto the stack for use by the RET instruction

@param state provides access to the state variable
@param opcode provides the address to jump to
*/
void CALL(State8080 *state, unsigned char *opcode);
/*
Pops the last address saved on stack into pc,
causing program transfer to that address

@param state provides access to the state variable
*/
void RET(State8080 *state);

/*LOGICAL GROUP*/

/*
Performs the logical AND operation with the accumulator [A]
pass in variable
effects flags (clear carry flag)

@param state  gives access to the state structure
@param var    provides the value to AND with [A]

*/
void ANA(State8080 *state, uint8_t var);
/*
Performs the logical OR operation with the accumulator [A]
pass in variable
effects flags (clear carry flag)

@param state  gives access to the state structure
@param var    provides the value to AND with [A]

*/
void ORA(State8080 *state, uint8_t var);
/*
Performs the logical XOR operation with the accumulator [A]
pass in variable
effects flags (clear carry flag)

@param state  gives access to the state structure
@param var    provides the value to AND with [A]

*/
void XRA(State8080 *state, uint8_t var);
/*
Performs a right bit shift of the accumulator
effects the CY flag (stores LSB in it)

@param state  gives access to the state structure
*/
void RRC(State8080 *state);
/*
Performs a left bit shift of the accumulator
effects the CY flag (stores MSB in it)

@param state  gives access to the state structure
*/
void RLC(State8080 *state);
/*
Performs a rotate right thru carry of the accumulator
effects the CY flag (stores previous LSB)

@param state  gives access to the state structure
*/
void RAR(State8080 *state);
/*
Performs a rotate left thru carry of the accumulator
effects the CY flag (stores previous MSB)

@param state  gives access to the state structure
*/
void RAL(State8080 *state);
/*
Compares the current value in register A with an
input variable

Set zero flag to 1 if equal
Set carry flag to 1 if A < var
Set carry flag to 0 if A > var

@param state   state register
@param var     the variable to be compared to A

*/
void CMP(State8080 *state, uint8_t var);

/*STACK GROUP*/
/*
Performs a POP of the stack, storing it in register
Works in reverse order, pointer increments when popping (Top of Stack is at SP)
Incrementing goes down the stack
Low-order is on top of stack

@param state   gives access to the state structure
@param regHigh gives access to the high-order register of register pair
@param regLow  gives access to the low-order register of register pair

NOTE: the low-order register is the second in a pair (ex. in BC, it is C)
*/
void POP(State8080* state, uint8_t *regHigh, uint8_t *regLow);
/*
Performs a PUSH of the stack
Works in reverse order, pointer decrements when popping (Top of Stack is at SP)
Decrementing goes up the stack
Low-order is on top of stack

@param state   gives access to the state structure
@param regHigh value to be pushed in from the high-order register of register pair
@param regLow  value to be pushed in from the low-order register of register pair

NOTE: the low-order register is the second in a pair (ex. in BC, it is C)
*/
void PUSH(State8080* state, uint8_t regHigh, uint8_t regLow);
/*
Performs a POP of the stack, storing it in the Pop Processor Status Word
Stores individual bits in the flag and in A

@param state   gives access to the state structure
*/
void POP_PSW(State8080 *state);
/*
Performs a PUSH of the stack, addning the flag values as a word in the stack
@param state   gives access to the state structure
*/
void PUSH_PSW(State8080 *state);
/*
moves contents of HL to SP
@param state   gives access to the state structure
*/
void SPHL(State8080* state);
/*
exchanges stack top with HL
@param state   gives access to the state structure
*/
void XTHL(State8080* state);
/*
exchanges reg l with reg e
exhanges reg h with reg d
@param state   gives access to the state structure
*/
void XCHG(State8080* state);
