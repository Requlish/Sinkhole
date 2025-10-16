//------------------------------------------------------------------------------------
// file: defeat.c
//
// authors:
// l.zheneudamon (primary author)
//		Added defeat Button Selector
//
// brief:
// Contains functions used for defeat menu.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

#include "cprocessing.h"
#include "utils.h"

int GetDefeatButtonSelected(float maxY) {
	float respawnButtonX = CP_System_GetWindowWidth() / 2.0f, respawnButtonY = maxY + 100, respawnButtonWidth = 600, respawnButtonHeight = 100;
	float mainmenuButtonX = CP_System_GetWindowWidth() / 2.0f, mainmenuButtonY = maxY + 200, mainmenuButtonWidth = 600, mainmenuButtonHeight = 100;
	float screenShift = maxY - CP_System_GetWindowHeight() / 2.0f;

	if (IsAreaClicked(respawnButtonX, respawnButtonY, respawnButtonWidth, respawnButtonHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY() + screenShift)) {
		return 1;
	} else if (IsAreaClicked(mainmenuButtonX, mainmenuButtonY, mainmenuButtonWidth, mainmenuButtonHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY() + screenShift)) {
		return 2;
	} else {
		return 0;
	}
}
