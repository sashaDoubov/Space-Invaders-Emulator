#pragma once
#include "..\Header Files\Opcodes.h"


/*
Main switch statement for interpreting opcodes

@param state	structure emulating CPU memory
@return		# of cycles (int) per given opcode

*/
int Emulate(State8080* state);