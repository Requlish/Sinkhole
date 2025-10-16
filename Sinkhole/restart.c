//------------------------------------------------------------------------------------
// file: restart.c
//
// authors:
// timo.duethorn (primary author)
//		Added Auto SetNextGameState to game.c
//
// brief:
// Handles restarting the game after defeat.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

#include <time.h>
#include <stdlib.h> 
#include "cprocessing.h"
#include "game.h"

void Restart_Init(void) {
	srand((unsigned int)time(NULL));
}

void Restart_Update(void) {
	CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);
}

void Restart_Exit(void) {

}
