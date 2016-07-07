// main.cpp : Defines the entry point for the console application.

//local includes
#include "stdafx.h"
#include "..\Header Files\emuProcessor.h"
#include "..\Header Files\SpaceInvadersMachine.h"
#include "..\Header Files\Opcodes.h"
#include <iostream>
#include <windows.h>

//necessary external libraries
#include <chrono>
#include <thread>
#include <cstdio>


// Declare initial condition codes
ConditionCodes CC_ZSPAC = { 1,1,1,0,1 };


/* CPU timer that ensures a 2 MHz running time
used in thread
@param machine gives access to space invaders machine
*/
void CPUtiming(SpaceInvadersMachine *machine)
{
	for (;;) {

		machine->doCPU();

		// Sleeps for 1 millisecond to ensure timing
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}


/*
Drawing of the screen
used in thread
takes in parameters necessary for drawing screen
runs at 60 Hz
*/
void Drawing(SpaceInvadersMachine *machine, SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture,Uint32 *pixels)
{
	for(;;){

		// clear screen (set all to white)
		memset(pixels, 255, 256 * 224 * sizeof(Uint32));
		
		// iterate over all the bytes stored in memory
		// Space Invaders is rotated by 90 deg by default, must be fixed
		for (int y = 0; y < 224; y++) {
			for (int x = 0; x < 256; x += 8) {
				
				uint8_t currentByte = machine->state->memory[0x2400 + y *(256 / 8) + x / 8];
				// split apart each of the screen bytes stored into their individual bits
				// a 0 means no change in the pixel (white)
				// a 1 means that it must be drawn (black)
				for (int i = 0; i < 8; i++)
				{
					// the current bit is black
					if (currentByte & 1)
					{
						// arithmetic to change it to be black, and rotate 90 deg
						 pixels[(255 - x - i) * 224 + y] = 0;
					}
					// iterate over next bit
					currentByte = currentByte >> 1;
				}

			}
		}
		

		SDL_UpdateTexture(texture, NULL, pixels, 224 * sizeof(Uint32));
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);

		// ensures 60 Hz timing
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	} 

}

// Windows Application
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{

	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_Texture * texture;

	// Safe initialization of SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return -1;
	}

	window = SDL_CreateWindow("Space Invaders",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 224, 256, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, 0);

	texture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, 224, 256);

	// machine object
	SpaceInvadersMachine *machine = new SpaceInvadersMachine();
	
	// SPACE INVADER FILES (find your own!)
	ReadFileIntoMemoryAt(machine->state, ".\\Rom Files\\invaders.h", 0);
	ReadFileIntoMemoryAt(machine->state, ".\\Rom Files\\invaders.g", 0x800);
	ReadFileIntoMemoryAt(machine->state, ".\\Rom Files\\invaders.f", 0x1000);
	ReadFileIntoMemoryAt(machine->state, ".\\Rom Files\\invaders.e", 0x1800);

	// pixels defined for the screen size
	Uint32 * pixels = new Uint32[256 * 224];

	// separate threads for CPU and for Display
	std::thread t1(CPUtiming, machine);
	std::thread t2(Drawing, machine, window,renderer,texture,pixels);

	// quirk of SDL2, must be defined in same thread as init video
	SDL_Event event;


	// Main event loop (runs keyboard)
	for (;;)
	{
		SDL_PollEvent(&event);

		switch (event.type) {
			case SDL_KEYDOWN:
				machine->keyDown(event.key.keysym.sym);
				break;
			case SDL_KEYUP:
				machine->keyUp(event.key.keysym.sym);
				break;
			case SDL_QUIT:
				SDL_Quit();
				exit(0);
				break;
		}
	}
	return 0;
}