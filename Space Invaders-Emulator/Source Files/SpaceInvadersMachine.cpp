#include "stdafx.h"
#include "..\Header Files\SpaceInvadersMachine.h"
#include "..\Header Files\Opcodes.h"
#include "..\Header Files\emuProcessor.h"
#include "SDL.h"

#include <chrono>
#include <cstdint>


/*
Constructor for space invaders

initializes the state machine
and various timers
*/
SpaceInvadersMachine::SpaceInvadersMachine(void)
{
	start_time = std::chrono::high_resolution_clock::now();
	state = Init8080();
	lastTimer = 0.0;
	whichInterrupt = 1; //going to be the first interrupt
}

/*
returns the running time of the space invaders machine
since running time, in microseconds.
*/
double SpaceInvadersMachine::time(void)
{
	auto current = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(current - start_time).count();
}


/* 
This code was heavily influenced by the Emulator 101 code
*/
void SpaceInvadersMachine::doCPU(void)
{

	double now = time();
	if (lastTimer == 0.0)
	{
		lastTimer = now;
		nextInterrupt = lastTimer + 16000.0;;
		whichInterrupt = 1;
	}

	if ((state->int_enable) && (now > nextInterrupt))
	{
		if (whichInterrupt == 1)
		{
			generateInterrupt(state, 1);
			whichInterrupt = 2;
		}
		else
		{
			generateInterrupt(state, 2);
			whichInterrupt = 1;
		}
		nextInterrupt = now + 8000.0;
	}


	//How much time has passed?  How many instructions will it take to keep up with
	// the current time?  Assume:
	//CPU is 2 MHz
	// so 2M cycles/sec

	// Rather than timing each instruction
	//make it fit into this loop
	double sinceLast = now - lastTimer;
	int cycles_to_catch_up = (int)2 * sinceLast;
	int cycles = 0;
	
	while (cycles_to_catch_up > cycles)
	{
		unsigned char *op;
		op = &state->memory[state->pc];
		if (*op == 0xdb) //machine specific handling for IN
		{
			uint8_t port = op[1];
			state->a = MachineIN(state,port);
			state->pc += 2;
			cycles += 3;
		}
		else if (*op == 0xd3) //machine specific handling for OUT
		{
			uint8_t port = op[1];
			MachineOUT(state, port,state->a);
			state->pc += 2;
			cycles += 3;
		}
		else
		{
			cycles += Emulate(state);
		}
	}
	lastTimer = now;

}


/*Used to generate an interrupt
keeps track of pc and changes it to a low-level address

@param state   provides access to the state
@param interrupt_num corrersponds to the
*/
void SpaceInvadersMachine::generateInterrupt(State8080 * state, int interrupt_num)
{
	//Push State
	PUSH(state, (state->pc & 0xFF00) >> 8, (state->pc & 0xff));

	//Basically perform a RST
	state->pc = 8 * interrupt_num;
	state->int_enable = 0;
}
/*
Emulates input port
*/
uint8_t SpaceInvadersMachine::MachineIN(State8080 * state, uint8_t port)
{

	switch (port)
	{
	case 0:
		return 1;
	case 1:
		return inPort1;
	case 2:
		return inPort2;
	case 3:
		uint16_t v = (shift1 << 8) | shift0;
		return ((v >> (8 - shift_offset)) & 0xff);
	}
}


/*
Emulates output port
*/
void SpaceInvadersMachine::MachineOUT(State8080 * state, uint8_t port,uint8_t val)
{
	switch (port)
	{
	case 2: 
		shift_offset = val & 0x7;
		break;
	case 4:
		shift0 = shift1;
		shift1 = val;
		break;
	}
}

/*
Corresponds to key press down
changes the corresponding bit in the program to a 1
More information can be found in webpage Space Invaders Archeology
*/
void SpaceInvadersMachine::keyDown(SDL_Keycode key)
{
	switch (key) {

	/*PORT 1*/
	// LEFT (1 player)
	case SDLK_LEFT:
		inPort1 |= 0x20;
		break;
	// RIGHT (1 player)
	case SDLK_RIGHT:
		inPort1 |= 0x40;
		break;
	// SHOOT (1 player)
	case SDLK_SPACE:
		inPort1 |= 0x10;
		break;
	// START (1 player)
	case SDLK_1:
		inPort1 |= 0x04;
		break;
	// START (2 player)
	case SDLK_2:
		inPort1 |= 0x02;
		break;
	// INSERT COIN
	case SDLK_c:
		inPort1 |= 0x1;
		break;

	/*PORT 2*/
	// LEFT (2 player)
	case SDLK_a:
		inPort2 |= 0x20;
		break;
	// RIGHT (2 player)
	case SDLK_d:
		inPort2 |= 0x40;
		break;
	// SHOOT (2 player)
	case SDLK_w:
		inPort2 |= 0x10;
		break;
	

	}
}

/*
Corresponds to key press release (up)
changes the corresponding bit in the program to a 0
More information can be found in webpage Space Invaders Archeology
*/
void SpaceInvadersMachine::keyUp(SDL_Keycode key)
{
	switch (key) {
		// LEFT (1 player)
		case SDLK_LEFT:
			inPort1 &= ~0x20;
			break;
		// RIGHT (1 player)
		case SDLK_RIGHT:
			inPort1 &= ~0x40;
			break;
		// SHOOT (1 player)
		case SDLK_SPACE:
			inPort1 &= ~0x10;
			break;
		// START (1 player)
		case SDLK_1:
			inPort1 &= ~0x04;
			break;
		// START (2 player)
		case SDLK_2:
			inPort1 &= ~0x02;
			break;
		// INSERT COIN
		case SDLK_c:
			inPort1 &= ~0x1;
			break;

		/*PORT 2*/
		// LEFT (2 player)
		case SDLK_a:
			inPort2 &= ~0x20;
			break;
		// RIGHT (2 player)
		case SDLK_d:
			inPort2 &= ~0x40;
			break;
		// SHOOT (2 player)
		case SDLK_w:
			inPort2 &= ~0x10;
			break;
	}
}



