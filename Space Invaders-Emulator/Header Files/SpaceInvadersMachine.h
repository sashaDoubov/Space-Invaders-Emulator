#pragma once
#include "SDL.h"
#include "..\Header Files\emuProcessor.h"
#include "..\stdafx.h"
#include "stdafx.h"
#include <chrono>
class SpaceInvadersMachine
{
public:
	State8080	*state;
	double		initialTime;
	double		lastTimer;
	double		nextInterrupt;
	int			whichInterrupt;

	std::chrono::high_resolution_clock::time_point	start_time;
	// LSB of Space Invader's external shift hardware
	uint8_t		shift0; 
	// MSB of Space Invader's external shift hardware
	uint8_t		shift1; 
	// offset for external hardware
	uint8_t		shift_offset; 
	// input port 1 used for key operations
	uint8_t		inPort1;
	// input port 2 used for key operations
	uint8_t		inPort2;
public:
	/*
	Constructor for space invaders

	initializes the state machine
	and various timers
	*/
	SpaceInvadersMachine::SpaceInvadersMachine(void);
	/*
	returns the running time of the space invaders machine
	since running time, in microseconds.
	*/
	double SpaceInvadersMachine::time(void);
	/*
	This code was heavily influenced by the Emulator 101 code
	*/
	void SpaceInvadersMachine::doCPU(void);
	/*Used to generate an interrupt
	keeps track of pc and changes it to a low-level address

	@param state   provides access to the state
	@param interrupt_num corrersponds to the
	*/
	void SpaceInvadersMachine::generateInterrupt(State8080 * state, int interrupt_num);
	/*
	Emulates input port
	*/
	uint8_t SpaceInvadersMachine::MachineIN(State8080 * state, uint8_t port);
	/*
	Emulates output port
	*/
	void SpaceInvadersMachine::MachineOUT(State8080 * state, uint8_t port, uint8_t val);
	/*
	Corresponds to key press down
	changes the corresponding bit in the program to a 1
	More information can be found in webpage Space Invaders Archeology
	*/
	void SpaceInvadersMachine::keyDown(SDL_Keycode key);
	/*
	Corresponds to key press release (up)
	changes the corresponding bit in the program to a 0
	More information can be found in webpage Space Invaders Archeology
	*/
	void SpaceInvadersMachine::keyUp(SDL_Keycode key);
};