//------------------------------------------------------------------------------------
// file: splashscreen.c
//
// authors:
// timo.duethorn (primary author)
//		Added Content of Splash Screen (Fading Logo & Text)
//
// brief:
// Contains the code to load and render the Splash Screen, then auto navigate to the Main Menu.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

#include "cprocessing.h"
#include "mainmenu.h"

CP_Image digipenLogo, blackCover;
CP_Font sigmaFont;
CP_Color black, white;

float deltaTime;
float counter = 0.0f;
float logoAlpha = 0.0f;
float sigmaAlpha = 255.0f;
float windowWidth, windowHeight;

void Splash_Screen_Init(void) {
	digipenLogo = CP_Image_Load("Assets/Images/DigiPen/DigiPen_Singapore_WEB_WHITE.png");
	blackCover = CP_Image_Load("Assets/Images/Background/black.png");
	sigmaFont = CP_Font_Load("Assets/Fonts/RussoOne-Regular.ttf");
	black = CP_Color_Create(0, 0, 0, 255);
	white = CP_Color_Create(255, 255, 255, 255);

	windowWidth = (float)CP_System_GetWindowWidth();
	windowHeight = (float)CP_System_GetWindowHeight();

	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_TextSize(400.0f);
	CP_Settings_Fill(white);
	CP_Font_Set(sigmaFont);
}

void Splash_Screen_Update(void) {
	CP_Graphics_ClearBackground(black);
	deltaTime = CP_System_GetDt();
	counter = counter + deltaTime;

	logoAlpha = counter < 2.0f ? logoAlpha + 255.0f / (2.0f / deltaTime) : logoAlpha - 255.0f / (2.0f / deltaTime);
	CP_Image_Draw(digipenLogo, windowWidth / 2.0f, windowHeight / 2.0f, 1220, 356, (int)logoAlpha);

	if (counter > 4.0f) {
		CP_Font_DrawText("SIGMA", windowWidth / 2.0f, windowHeight / 2.0f);
		sigmaAlpha = counter < 6.0f ? sigmaAlpha - 255.0f / (2.0f / deltaTime) : sigmaAlpha + 255.0f / (2.0f / deltaTime);
		CP_Image_Draw(blackCover, windowWidth / 2.0f, windowHeight / 2.0f, windowWidth, windowHeight, (int)sigmaAlpha);
	}

	if (counter > 8.0f || CP_Input_KeyTriggered(KEY_ESCAPE)) { // ESC Key Shortcut
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	}
}

void Splash_Screen_Exit(void) {
	CP_Image_Free(&digipenLogo);
	CP_Image_Free(&blackCover);
}
