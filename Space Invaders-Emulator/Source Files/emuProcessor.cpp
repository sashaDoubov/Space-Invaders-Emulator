#include "stdafx.h"
#include "..\Header Files\Opcodes.h"
#include "..\Header Files\SpaceInvadersMachine.h"

/*
Array storing the # of cycles for each opcode
cycle8080[opcode #] = cycle time of that operation
*/
unsigned char cycles8080[] = {
	4, 10, 7, 5, 5, 5, 7, 4, 4, 10, 7, 5, 5, 5, 7, 4, //0x00..0x0f
	4, 10, 7, 5, 5, 5, 7, 4, 4, 10, 7, 5, 5, 5, 7, 4, //0x10..0x1f
	4, 10, 16, 5, 5, 5, 7, 4, 4, 10, 16, 5, 5, 5, 7, 4, //etc
	4, 10, 13, 5, 10, 10, 10, 4, 4, 10, 13, 5, 5, 5, 7, 4,

	5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5, //0x40..0x4f
	5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5,
	5, 5, 5, 5, 5, 5, 7, 5, 5, 5, 5, 5, 5, 5, 7, 5,
	7, 7, 7, 7, 7, 7, 7, 7, 5, 5, 5, 5, 5, 5, 7, 5,

	4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4, //0x80..8x4f
	4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
	4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,
	4, 4, 4, 4, 4, 4, 7, 4, 4, 4, 4, 4, 4, 4, 7, 4,

	11, 10, 10, 10, 17, 11, 7, 11, 11, 10, 10, 10, 10, 17, 7, 11, //0xc0..0xcf
	11, 10, 10, 10, 17, 11, 7, 11, 11, 10, 10, 10, 10, 17, 7, 11,
	11, 10, 10, 18, 17, 11, 7, 11, 11, 5, 10, 5, 17, 17, 7, 11,
	11, 10, 10, 4, 17, 11, 7, 11, 11, 5, 10, 4, 17, 17, 7, 11,
};


int Emulate(State8080* state)
{
	int cycles = 4;
	unsigned char *opcode = &state->memory[state->pc];
	state->pc += 1;

	switch (*opcode)
	{
	case 0x00:			// NOP
		break; 
	case 0x01:			// LXI B,D16
		state->c = opcode[1];
		state->b = opcode[2];
		state->pc += 2;
		break;
	case 0x02:			// STAX B
		{
			uint16_t bc = (state->b << 8) | (state->c);
			state->memory[bc] = state->a;
		}
		break;
	case 0x03:			// INX B
		INX(&state->b, &state->c);
		break;
	case 0x04:			// INR B
		INR(state, &state->b);
		break;
	case 0x05:			// DCR B
		DCR(state, &state->b);
		break;
	case 0x06:			// MVI B
		state->b = opcode[1];
		state->pc++;
		break;
	case 0x07:			// RLC
		RLC(state);
		break;
	case 0x09:			// DAD B
		{
			uint32_t bc = (state->b << 8) | (state->c);
			DAD(state, bc);
		}
		break;
	case 0x0a:			// LDAX B
		{
			uint16_t bc = (state->b << 8) | (state->c);
			state->a = state->memory[bc];
		}
		break;
	case 0x0b:			// DCX B
		DCX(&state->b, &state->c);
		break;
	case 0x0c:			// INR C
		INR(state, &state->c);
		break;
	case 0x0d:			// DCR C
		DCR(state, &state->c);
		break;
	case 0x0e:			// MVI C
		state->c = opcode[1];
		state->pc++;
		break;
	case 0x0f:			// RRC
		RRC(state);
		break;
	case 0x11:			// LXI D,D16
		state->d = opcode[2];
		state->e = opcode[1];
		state->pc += 2;
		break;
	case 0x12:			// STAX D
		{
			uint16_t de = (state->d << 8) | (state->e);
			state->memory[de] = state->a;
		}
		break;
	case 0x13:			// INX D
		INX(&state->d, &state->e);
		break;
	case 0x14:			// INR D
		INR(state, &state->d);
		break;
	case 0x15:			// DCR D
		DCR(state, &state->d);
		break;
	case 0x16:			// MVI D
		state->d = opcode[1];
		state->pc++;
		break;
	case 0x17:			// RAL
		RAL(state);
		break;
	case 0x19:			// DAD D
		{
			int32_t de = (state->d << 8) | (state->e);
			// pass by value
			DAD(state, de);
		}
		break;
	case 0x1a:			// LDAX D
		{
			uint16_t de = (state->d << 8) | (state->e);
			state->a = state->memory[de];
		}
		break;
	case 0x1b:			// DCX D
		DCX(&state->d, &state->e);
		break;
	case 0x1c:			// INR E;
		INR(state, &state->e);
		break;
	case 0x1d:			// DCR E
		DCR(state, &state->e);
		break;
	case 0x1e:			// MVI E
		state->e = opcode[1];
		state->pc++;
		break;
	case 0x1f:			// RAR
		RAR(state);
		break;
	case 0x20: printf("RIM"); break;
	case 0x21:			// LXI H,D16
		state->h = opcode[2];
		state->l = opcode[1];
		state->pc += 2;
		break;
	case 0x22:			// SHLD adr
		{
			uint16_t adr = (opcode[2] << 8) | opcode[1];
			state->memory[adr] = state->l;
			state->memory[adr + 1] = state->h;
			state->pc += 2;
		}
		break;
	case 0x23:			// INX H
		INX(&state->h, &state->l);
		break;
	case 0x24:			// INR H
		INR(state, &state->h);
		break;
	case 0x25:			// DCR H
		DCR(state, &state->h);
		break;
	case 0x26:			// MVI H
		state->h = opcode[1];
		state->pc++;
		break;
	case 0x27:			// DAA
		if ((state->a & 0xf) > 9)
			state->a += 6;
		if ((state->a & 0xf0) > 0x90)
		{
			uint16_t res = (uint16_t)state->a + 0x60;
			state->a = res & 0xff;
			Flags(state, res);
		}
		break;
	case 0x29:			// DAD H
		{
			uint32_t hl = (state->h << 8) | (state->l);
			DAD(state, hl);
		}
		break;
	case 0x2a:			// LHLD adr
		{
			uint16_t adr = (opcode[2] << 8) | (opcode[1]);
			state->l = state->memory[adr];
			state->h = state->memory[adr + 1];
			state->pc += 2;
		}
		break;
	case 0x2b:			// DCX H
	{
		DCX(&state->h, &state->l);
	}
	break;
	case 0x2c:			// INR L
		INR(state, &state->l);
		break;
	case 0x2d:			// DCR L;
		DCR(state, &state->l);
		break;
	case 0x2e:			// MVI L,D8
		state->l = opcode[1];
		state->pc++;
		break;
	case 0x2f:			// CMA
		// complements state a
		state->a = ~(state->a);
		break;
	case 0x30: printf("SIM"); break;
	case 0x31:			// LXI SP, D16
		state->sp = (opcode[2] << 8) | opcode[1];
		state->pc += 2;
		break;
	case 0x32:			// STA adr
	{
		uint16_t adr = (opcode[2] << 8) | (opcode[1]);
		state->memory[adr] = state->a;
		state->pc += 2;
	}
	break;
	case 0x33:			// INX SP
		++(state->sp);
		break;
	case 0x34:			// INR M
		{
			uint16_t offset_ = (state->h << 8) | (state->l);
			INR(state, &state->memory[offset_]);
		}
		break;
	case 0x35:			// DCR M
		{
			uint16_t offset_ = (state->h << 8) | (state->l);
			DCR(state, &state->memory[offset_]);
		}
		break;
	case 0x36:			// MVI M,D8
		{
			uint16_t offset_ = (state->h << 8) | (state->l);
			state->memory[offset_] = opcode[1];
			state->pc++;
		}
		break;
	case 0x37:			// STC
						// set carry to be 1
		state->cc.cy = 1;
		break;
	case 0x39:			// DAD SP
		{
			DAD(state, state->sp);
		}
		break;
	case 0x3a:			// LDA adr
		{
			uint16_t adr = (opcode[2] << 8) | (opcode[1]);
			state->a = state->memory[adr];
			state->pc += 2;
		}
		break;
	case 0x3b:			// DCX SP
		--(state->sp);
		break;
	case 0x3c:			// INR A
		INR(state, &state->a);
		break;
	case 0x3d:			// DCR A
		DCR(state, &state->a);
		break;
	case 0x3e:			// MVI A,D8
		state->a = opcode[1];
		state->pc++;
		break;
	case 0x3f:			// CMC
		// complements the carry bit
		state->cc.cy = ~(state->cc.cy);
		break;
	case 0x40:			// MOV B,B
		state->b = state->b;
		break;
	case 0x41:			// MOV B,C
		state->b = state->c;
		break;
	case 0x42:			// MOV B,D
		state->b = state->d;
		break;
	case 0x43:			// MOV B,E
		state->b = state->e;
		break;
	case 0x44:			// MOV B,H
		state->b = state->h;
		break;
	case 0x45:			// MOV B,L
		state->b = state->l;
		break;
	case 0x46:			//MOV B,M
		{
			uint16_t offset_ = (state->h << 8) | (state->l);
			state->b = state->memory[offset_];
		}
		break;
	case 0x47:			// MOV B,A
		state->b = state->a;
		break;
	case 0x48:			//MOV C,B
		state->c = state->b;
		break;
	case 0x49:			//MOV C,C
		state->c = state->c;
		break;
	case 0x4a:			//MOV C,D
		state->c = state->d;
		break;
	case 0x4b:			//MOV C,E
		state->c = state->e;
		break;
	case 0x4c:			//MOV C,H
		state->c = state->h;
		break;
	case 0x4d:			//MOV C,L
		state->c = state->l;
		break;
	case 0x4e:			//MOV C,M
		{
			uint16_t offset_ = (state->h << 8) | (state->l);
			state->c = state->memory[offset_];
		}
		break;
	case 0x4f:			//MOV C,A
		state->c = state->a;
		break;
	case 0x50:			// MOV D,B
		state->d = state->b;
		break;
	case 0x51:			// MOV D,C
		state->d = state->c;
		break;
	case 0x52:			// MOV D,D
		state->d = state->d;
		break;
	case 0x53:			// MOV D,E
		state->d = state->e;
		break;
	case 0x54:			// MOV D,H
		state->d = state->h;
		break;
	case 0x55:			// MOV D,L
		state->d = state->l;
		break;
	case 0x56:			//MOV D,M
		{
			uint16_t offset_ = (state->h << 8) | (state->l);
			state->d = state->memory[offset_];
		}
		break;
	case 0x57:			// MOV D,A
		state->d = state->a;
		break;
	case 0x58:			// MOV E,B
		state->e = state->b;
		break;
	case 0x59:			// MOV E,C
		state->e = state->c;
		break;
	case 0x5a:			// MOV E,D
		state->e = state->d;
		break;
	case 0x5b:			// MOV E,E
		state->e = state->e;
		break;
	case 0x5c:			// MOV E,H
		state->e = state->h;
		break;
	case 0x5d:			// MOV E,L
		state->e = state->l;
		break;
	case 0x5e:			//MOV E,M
		{
			uint16_t offset_ = (state->h << 8) | (state->l);
			state->e = state->memory[offset_];
		}
		break;
	case 0x5f:			// MOV E,A
		state->e = state->a;
		break;
	case 0x60:			// MOV H,B
		state->h = state->b;
		break;
	case 0x61:			// MOV H,C
		state->h = state->c;
		break;
	case 0x62:			// MOV H,D
		state->h = state->d;
		break;
	case 0x63:			// MOV H,E
		state->h = state->e;
		break;
	case 0x64:			// MOV H,H
		state->h = state->h;
		break;
	case 0x65:			// MOV H,L
		state->h = state->l;
		break;
	case 0x66:			//MOV H,M
		{
			uint16_t offset_ = (state->h << 8) | (state->l);
			state->h = state->memory[offset_];
		}
		break;
	case 0x67:			// MOV H,A
		state->h = state->a;
		break;
	case 0x68:			// MOV L,B
		state->l = state->b;
		break;
	case 0x69:			// MOV L,C
		state->l = state->c;
		break;
	case 0x6a:			// MOV L,D
		state->l = state->d;
		break;
	case 0x6b:			// MOV L,E
		state->l = state->e;
		break;
	case 0x6c:			// MOV L,H
		state->l = state->h;
		break;
	case 0x6d:			// MOV L,L
		state->l = state->l;
		break;
	case 0x6e:			//MOV L,M
		{
			uint16_t offset_ = (state->h << 8) | (state->l);
			state->l = state->memory[offset_];
		}
		break;
	case 0x6f:			// MOV L,A
		state->l = state->a;
		break;
	case 0x70:			//MOV M,B
		{
			uint16_t offset_ = (state->h << 8) | (state->l);
			state->memory[offset_] = state->b;
		}
		break;
	case 0x71:			//MOV M,C
		{
			uint16_t offset_ = (state->h << 8) | (state->l);
			state->memory[offset_] = state->c;
		}
		break;
	case 0x72:			//MOV M,D
		{
			uint16_t offset_ = (state->h << 8) | (state->l);
			state->memory[offset_] = state->d;
		}
		break;
	case 0x73:			// MOV M,E
		{
			uint16_t offset_ = (state->h << 8) | (state->l);
			state->memory[offset_] = state->e;
		}
		break;
	case 0x74:			// MOV M,H
		{
			uint16_t offset_ = (state->h << 8) | (state->l);
			state->memory[offset_] = state->h;
		}
		break;
	case 0x75:			// MOV M,L
		{
			uint16_t offset_ = (state->h << 8) | (state->l);
			state->memory[offset_] = state->l;
		}
		break;
	case 0x76:			// HLT
		exit(0);
		break;
	case 0x77:			// MOV M,A
		{
			uint16_t offset_ = (state->h << 8) | (state->l);
			state->memory[offset_] = state->a;
		}
		break;
	case 0x78:			// MOV A,B
		state->a = state->b;
		break;
	case 0x79:			// MOV A,C
		state->a = state->c;
		break;
	case 0x7a:			// MOV A,D
		state->a = state->d;
		break;
	case 0x7b:			// MOV A,E
		state->a = state->e;
		break;
	case 0x7c:			// MOV A,H
		state->a = state->h;
		break;
	case 0x7d:			// MOV A,L
		state->a = state->l;
		break;
	case 0x7e:			// MOV A,M
		{
			uint16_t offset_ = (state->h << 8) | (state->l);
			state->a = state->memory[offset_];
		}
		break;
	case 0x7f:			// MOV A,A
		state->a = state->a;
		break;

	case 0x80:			// ADD B
		ADD(state, state->b);
		break;
	case 0x81:			// ADD C
		ADD(state, state->c);
		break;
	case 0x82:			// ADD D
		ADD(state, state->d);
		break;
	case 0x83:			// ADD E
		ADD(state, state->e);
		break;
	case 0x84:			// ADD H
		ADD(state, state->h);
		break;
	case 0x85:			// ADD L
		ADD(state, state->l);
		break;
	case 0x86:			// ADD M
		{
			uint16_t offset_ = (state->h << 8) | (state->l);
			ADD(state, state->memory[offset_]);
		}
		break;
	case 0x87:			// ADD A
		ADD(state, state->a);
		break;
	case 0x88:			// ADC B
		ADC(state, state->b);
		break;
	case 0x89:			// ADC C
		ADC(state, state->c);
		break;
	case 0x8a:			// ADC D
		ADC(state, state->d);
		break;
	case 0x8b:			// ADC E
		ADC(state, state->e);
		break;
	case 0x8c:			// ADC H
		ADC(state, state->h);
		break;
	case 0x8d:			// ADC L
		ADC(state, state->l);
		break;
	case 0x8e:			// ADC M
		{
			uint16_t offset_ = (state->h << 8) | (state->l);
			ADC(state, state->memory[offset_]);
		}
		break;
	case 0x8f:			// ADC A
		ADC(state, state->a);
		break;
	case 0x90:			// SUB B
		SUB(state, state->b);
		break;
	case 0x91:			// SUB C
		SUB(state, state->c);
		break;
	case 0x92:			// SUB D
		SUB(state, state->d);
		break;
	case 0x93:			// SUB E
		SUB(state, state->e);
		break;
	case 0x94:			// SUB H
		SUB(state, state->h);
		break;
	case 0x95:			// SUB L
		SUB(state, state->l);
		break;
	case 0x96:			// SUB M
		{
			uint16_t offset_ = (state->h << 8) | (state->l);
			SUB(state, state->memory[offset_]);
		}
		break;
	case 0x97:			// SUB A
		SUB(state, state->a);
		break;
	case 0x98:			// SBB B
		SBB(state, state->b);
		break;
	case 0x99:			// SBB C
		SBB(state, state->c);
		break;
	case 0x9a:			// SBB D
		SBB(state, state->d);
		break;
	case 0x9b:			// SBB E
		SBB(state, state->e);
		break;
	case 0x9c:			// SBB H
		SBB(state, state->h);
		break;
	case 0x9d:			// SBB L
		SBB(state, state->l);
		break;
	case 0x9e:			// SBB M
		{
			uint16_t offset_ = (state->h << 8) | (state->l);
			SBB(state, state->memory[offset_]);
		}
		break;
	case 0x9f:			// SBB A
		SBB(state, state->a);
		break;
	case 0xa0:			// ANA B
		// performs the AND operation on register B
		ANA(state, state->b);
		break;
	case 0xa1:			// ANA C
		// performs the AND operation on register C
		ANA(state, state->c);
		break;
	case 0xa2:			// ANA D
		// performs the AND operation on register D
		ANA(state, state->d);
		break;
	case 0xa3:			// ANA E
		// performs the AND operation on register E
		ANA(state, state->e);
		break;
	case 0xa4:			// ANA H
		// performs the AND operation on register H
		ANA(state, state->h);
		break;
	case 0xa5:			// ANA L
		// performs the AND operation on register L
		ANA(state, state->l);
		break;
	case 0xa6:			// ANA M
		{
			// performs the AND operation of content stored by address at (HL)
			uint16_t offset_ = (state->h << 8) | (state->l);
			// cast to 8 bit, since no carry
			ANA(state, (uint8_t)state->memory[offset_]);
		}
		break;
	case 0xa7:			// ANA A
		// performs the AND operation on register A
		ANA(state, state->a);
		break;
	case 0xa8:			// XRA B
		// performs the XOR operation on register B
		XRA(state, state->b);
		break;
	case 0xa9:			// XRA C
		// performs the XOR operation on register C
		XRA(state, state->c);
		break;
	case 0xaa:			// XRA D
		// performs the XOR operation on register D
		XRA(state, state->d);
		break;
	case 0xab:			// XRA E
		// performs the XOR operation on register E
		XRA(state, state->e);
		break;
	case 0xac:			// XRA H
		// performs the XOR operation on register H
		XRA(state, state->h);
		break;
	case 0xad:			// XRA L
		// performs the XOR operation on register L
		XRA(state, state->l);
		break;
	case 0xae:			// XRA M
		{
			// performs the XOR operation of content stored by address at (HL)
			uint16_t offset_ = (state->h << 8) | (state->l);
			// cast to 8 bit, since no carry
			XRA(state, (uint8_t)state->memory[offset_]);
		}
		break;
	case 0xaf:			// XRA A
		// performs the XOR operation on register A
		XRA(state, state->a);
		break;
	case 0xb0:			// ORA B
		// performs the OR operation on register B
		ORA(state, state->b);
		break;
	case 0xb1:			// ORA C
		// performs the OR operation on register C
		ORA(state, state->c);
		break;
	case 0xb2:			// ORA D
		// performs the OR operation on register D
		ORA(state, state->d);
		break;
	case 0xb3:			// ORA E
		// performs the OR operation on register E
		ORA(state, state->e);
		break;
	case 0xb4:			// ORA H
		// performs the OR operation on register H
		ORA(state, state->h);
		break;
	case 0xb5:			// ORA L
		// performs the OR operation on register B
		ORA(state, state->l);
		break;
	case 0xb6:			// ORA M
		{
			// performs the OR operation of content stored by address at (HL)
			uint16_t offset_ = (state->h << 8) | (state->l);
			// cast to 8 bit, since no carry
			ORA(state, (uint8_t)state->memory[offset_]);
		}
		break;
	case 0xb7:			// ORA A
			   // performs the OR operation on register A
		ORA(state, state->a);
		break;
	case 0xb8:			// CMP B
		//compares value in accumulator with reg B value
		CMP(state, state->b);
		break;
	case 0xb9:			// CMP C
		//compares value in accumulator with reg C value
		CMP(state, state->c);
		break;
	case 0xba:			// CMP D
		//compares value in accumulator with reg D value
		CMP(state, state->d);
		break;
	case 0xbb:			// CMP E
		//compares value in accumulator with reg E value
		CMP(state, state->e);
		break;
	case 0xbc:			// CMP H
		//compares value in accumulator with reg H value
		CMP(state, state->h);
		break;
	case 0xbd:			// CMP L
		//compares value in accumulator with reg L value
		CMP(state, state->l);
		break;
	case 0xbe:			// CMP M
		{
			//compares value in accumulator with content stored by address at (HL)
			uint16_t offset_ = (state->h << 8) | (state->l);
			// cast to 8 bit, since no carry
			CMP(state, (uint8_t)state->memory[offset_]);
		}
			   break;
	case 0xbf:			// CMP A
		//compares value in accumulator with reg A value
		CMP(state, state->a);
		break;

	case 0xc0:			// RNZ
		//jump if not zero
		if (state->cc.z == 0)
		{
			RET(state);
		}
		break;
	case 0xc1:			// POP B
		POP(state, &state->b, &state->c);
		break;
	case 0xc2:			// JNZ adr
		// if zero condtion is zero ( not zero) jump to next adress
		if (state->cc.z == 0)
		{
			state->pc = opcode[2] << 8 | opcode[1];
		}
		else
		{
			// skip over the next two inputs
			state->pc += 2;
		}
		break;
	case 0xc3:			// JMP adr
		state->pc = (opcode[2] << 8) | opcode[1];
		break;
	case 0xc4:			// CNZ adr
		if (state->cc.z == 0)
		{
			CALL(state, opcode);
		}
		else
		{
			// skip over the next two inputs
			state->pc += 2;
		}
		break;
	case 0xc5:			// PUSH B
		PUSH(state, state->b, state->c);
		break;
	case 0xc6:			// ADI D8
		ADD(state, opcode[1]);
		state->pc++;
		break;
	case 0xc7:			// RST 0
	// CALL $0
		{
			unsigned char temp[3];
			temp[1] = 0;
			temp[2] = 0;
			CALL(state, temp);
		}
		break;
	case 0xc8:			// RZ
		//jump if zero
		if (state->cc.z == 1)
		{
			RET(state);
		}
		break;
	case 0xc9:
		RET(state);
		break;
	case 0xca:			// JZ adr
		if (state->cc.z == 1)
		{
			state->pc = (opcode[2] << 8) | opcode[1];
		}
		else
		{
			// skip over the next two inputs
			state->pc += 2;
		}
		break;
	case 0xcc:			// CZ adr
		// calls if zero
		if (state->cc.z == 1)
		{
			CALL(state, opcode);
		}
		else
		{
			// skip over the next two inputs
			state->pc += 2;
		}
		break;
	case 0xcd:			// CALL adr
		// stores the address after the call onto the stack in memory
		CALL(state, opcode);
		break;
	case 0xce:			// ACI D8
		ADC(state, opcode[1]);
		state->pc++;
		break;
	case 0xcf:			// RST 1"
		// CALL $10
		{
			unsigned char temp[3];
			temp[1] = 8;
			temp[2] = 0;
			CALL(state, temp);
		}
		break;
	case 0xd0:			// RNC
		//jump if NOT carry
		if (state->cc.cy == 0)
		{
			RET(state);
		}
		break;
	case 0xd1:			// POP D
		POP(state, &state->d, &state->e);
		break;
	case 0xd2:			// JNC adr
		// if there is no carry
		if (state->cc.cy == 0)
		{
			state->pc = opcode[2] << 8 | opcode[1];
		}
		else
		{
			// skip over the next two inputs
			state->pc += 2;
		}
		break;
	case 0xd3:			// OUT
		state->pc++;
		break;
	case 0xd4:			// CNC adr
		// call if NOT carry
		if (state->cc.cy == 0)
		{
			CALL(state, opcode);
		}
		else
		{
			// skip over the next two inputs
			state->pc += 2;
		}
		break;
	case 0xd5:			// PUSH D
		PUSH(state, state->d, state->e);
		break;
	case 0xd6:			// SUI c
		SUB(state, opcode[1]);
		state->pc++;
		break;
	case 0xd7:			// RST 2
		// CALL $10
		{
			unsigned char temp[3];
			temp[1] = 16;
			temp[2] = 0;
			CALL(state, temp);
		}
		break;
	case 0xd8:			// RC
		if (state->cc.cy != 0)
		{
			RET(state);
		}
		break;
	case 0xda:			// JC adr
		if (state->cc.cy != 0)
		{
			state->pc = opcode[2] << 8 | opcode[1];
		}
		else
		{
			// skip over the next two inputs
			state->pc += 2;
		}
		break;
	case 0xdb:			// IN
		state->pc++;
		break;
	case 0xdc:			// CC adr
		if (state->cc.cy == 1)
		{
			CALL(state, opcode);
		}
		else
		{
			// skip over the next two inputs
			state->pc += 2;
		}
		break;
	case 0xde:			// SBI D8
		SBB(state, opcode[1]);
		state->pc++;
		break;
	case 0xdf:			// RST 3
	// CALL $18
		{
			unsigned char temp[3];
			temp[1] = 24;
			temp[2] = 0;
			CALL(state, temp);
		}
		break;

	case 0xe0:			// RPO
		if (state->cc.p == 0)
		{
			RET(state);
		}
		break;
	case 0xe1:			// POP H
		POP(state, &state->h, &state->l);
		break;
	case 0xe2:			// JPO adr
		if (state->cc.p == 0)
		{
			state->pc = opcode[2] << 8 | opcode[1];
		}
		else
		{
			// skip over the next two inputs
			state->pc += 2;
		}
		break;
	case 0xe3:			// XTHL
		XTHL(state);
		break;
	case 0xe4:			// CPO adr
		if (state->cc.p == 0)
		{
			CALL(state, opcode);
		}
		else
		{
			// skip over the next two inputs
			state->pc += 2;
		}
		break;
	case 0xe5:			// PUSH H
		PUSH(state, state->h, state->l);
		break;
	case 0xe6:			// ANI D8
		// performs the AND operation with the 
		//next byte of the instruction
		ANA(state, opcode[1]);
		state->pc++;
		break;
	case 0xe7:			// RST 4
		// CALL $20
		{
			unsigned char temp[3];
			temp[1] = 32;
			temp[2] = 0;
			CALL(state, temp);
		}
		break;
	case 0xe8:			// RPE
		if (state->cc.p == 1)
		{
			RET(state);
		}
		break;
	case 0xe9:			// PCHL
		state->pc = (state->h << 8) | (state->l);
		break;
	case 0xea:			// JPE adr
		if (state->cc.p != 0)
		{
			state->pc = opcode[2] << 8 | opcode[1];
		}
		else
		{
			// skip over the next two inputs
			state->pc += 2;
		}
		break;
	case 0xeb:			// XCHG
		XCHG(state);
		break;
	case 0xec:			// CPE adr
		// if parity is even, call
		if (state->cc.p != 0)
		{
			CALL(state, opcode);
		}
		else
		{
			// skip over the next two inputs
			state->pc += 2;
		}
		break;
	case 0xee:			// XRI D8
		// performs the XOR operation with the 
		// next byte of the instruction
		XRA(state, opcode[1]);
		state->pc++;
		break;
	case 0xef:			// RST 5
		// CALL $28
		{
			unsigned char temp[3];
			temp[1] = 40;
			temp[2] = 0;
			CALL(state, temp);
		}
		break;
	case 0xf0:			// RP
		// if positive ret
		if (state->cc.s == 0)
		{
			RET(state);
		}
		break;
	case 0xf1:			// POP PSW
		POP_PSW(state);
		break;
	case 0xf2:			// JP adr
		if (state->cc.s == 0)
		{
			state->pc = opcode[2] << 8 | opcode[1];
		}
		else
		{
			// skip over the next two inputs
			state->pc += 2;
		}
		break;
	case 0xf3:			// DI
		state->int_enable = 0;
		break;
	case 0xf4:			// CP adr
		//call if positive
		if (state->cc.s == 0)
		{
			CALL(state, opcode);
		}
		else
		{
			// skip over the next two inputs
			state->pc += 2;
		}
		break;
	case 0xf5:			// PUSH PSW
		PUSH_PSW(state);
		break;
	case 0xf6:			// ORI D8
		// performs the OR operation with the 
		//next byte of the instruction
		ORA(state, opcode[1]);
		state->pc++;
		break;
	case 0xf7:			// RST 6
		//CALL $30
		{
			unsigned char temp[3];
			temp[1] = 48;
			temp[2] = 0;
			CALL(state, temp);
		}
		break;
	case 0xf8:			// RM
		// if minus ret
		if (state->cc.s != 0)
		{
			RET(state);
		}
		break;
	case 0xf9:			// SPHL
		SPHL(state);
		break;
	case 0xfa:			// JM adr
		if (state->cc.s == 1)
		{
			state->pc = opcode[2] << 8 | opcode[1];
		}
		else
		{
			// skip over the next two inputs
			state->pc += 2;
		}
		break;
	case 0xfb:			// EI
		state->int_enable = 1;
		break;
	case 0xfc:			// CM adr
		// call if negative (minus)
		if (state->cc.s == 1)
		{
			CALL(state, opcode);
		}
		else
		{
			// skip over the next two inputs
			state->pc += 2;
		}
		break;
	case 0xfe:			// CPI D8
		//compares value in accumulator with next value
		CMP(state, opcode[1]);
		// increment program counter
		state->pc++;
		break;
	case 0xff:			//RST 7
		//  CALL $38
		{
			unsigned char temp[3];
			temp[1] = 56;
			temp[2] = 0;
			CALL(state, temp);
		}
		break;

	}
	return cycles8080[*opcode];
}
