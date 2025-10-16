//------------------------------------------------------------------------------------
// file: main.c
//
// authors:
// Prof DX & Prof Gerald
//
// brief:
// Main entry point for the SinkHole game.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

#include "cprocessing.h"
#include "splashscreen.h"

// main() the starting point for the program
// CP_Engine_SetNextGameState() tells CProcessing which functions to use for init, update and exit
// CP_Engine_Run() is the core function that starts the simulation
int main(void) {
	CP_Engine_SetNextGameState(Splash_Screen_Init, Splash_Screen_Update, Splash_Screen_Exit);
	CP_System_SetWindowSize(1600, 900);
	CP_Engine_Run();
	return 0;
}
