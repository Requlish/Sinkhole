//------------------------------------------------------------------------------------
// file: pause.c
//
// authors:
// l.zheneudamon (primary author)
//		Added Pause Button Selector
//
// brief:
// Contains functions used for pause menu.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

#include "cprocessing.h"
#include "utils.h"

int GetPauseButtonSelected(float maxY) {
	float resumeButtonX = CP_System_GetWindowWidth() / 2.0f, resumeButtonY = maxY, resumeButtonWidth = 600, resumeButtonHeight = 100;
	float mainmenuButtonX = CP_System_GetWindowWidth() / 2.0f, mainmenuButtonY = maxY + 100, mainmenuButtonWidth = 600, mainmenuButtonHeight = 100;
	float exitButtonX = CP_System_GetWindowWidth() / 2.0f, exitButtonY = maxY + 200, exitButtonWidth = 600, exitButtonHeight = 100;
	float screenShift = maxY - CP_System_GetWindowHeight() / 2.0f;

	if (IsAreaClicked(resumeButtonX, resumeButtonY, resumeButtonWidth, resumeButtonHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY() + screenShift)) {
		return 1;
	} else if (IsAreaClicked(mainmenuButtonX, mainmenuButtonY, mainmenuButtonWidth, mainmenuButtonHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY() + screenShift)) {
		return 2;
	} else if (IsAreaClicked(exitButtonX, exitButtonY, exitButtonWidth, exitButtonHeight, CP_Input_GetMouseX(), CP_Input_GetMouseY() + screenShift)) {
		return 3;
	} else {
		return 0;
	}
}
