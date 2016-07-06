#pragma once
#include "stdafx.h"
/* CPU timer that ensures a 2 MHz running time
used in thread
@param machine gives access to space invaders machine
*/
void CPUtiming(SpaceInvadersMachine *machine);
/*
Drawing of the screen
used in thread
takes in parameters necessary for drawing screen
runs at 60 Hz
*/
void Drawing(SpaceInvadersMachine *machine, SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture, Uint32 *pixels);
int main(int argc, char*argv[]);