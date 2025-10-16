//------------------------------------------------------------------------------------
// file: mainmenu.c
//
// authors:
// timo.duethorn (primary author)
//		Added Content of Main Menu (Buttons & Text)
//		Added Drawing of Looping Background
//		Added Music
//
// brief:
// Contains the main logic of the Main Menu.
// Contains functionality of buttons to redirect to next game states.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h> 
#include <time.h> 
#include "cprocessing.h"
#include "game.h"
#include "credits.h"
#include "utils.h"

CP_Image backgroundImage;
CP_Sound menuTrack = NULL;
CP_Font titleFont, buttonFont;
CP_Color black, brown, lightBrown, white;

struct rect {
	float x;
	float y;
	float height;
	float width;
};
struct rect playRect;
struct rect exitRect;
struct rect creditsRect;

int leftMouseClick;
int buttonHovered = 0;
int backgroundSpeed;
int playMusicOnce;
float deltaTime;
float backgroundHeight1;
float backgroundHeight2;

void Main_Menu_Init(void) {
	backgroundImage = CP_Image_Load("Assets/Images/Background/background.png");

	menuTrack = CP_Sound_LoadMusic("Assets/Music/lost_civilization.mp3");

	leftMouseClick = 0;
	playMusicOnce = 0;

	black = CP_Color_Create(0, 0, 0, 255);
	brown = CP_Color_Create(65, 40, 4, 255);
	white = CP_Color_Create(255, 255, 255, 255);
	lightBrown = CP_Color_Create(245, 222, 179, 255);

	playRect.x = CP_System_GetWindowWidth() / 2.0f, playRect.y = CP_System_GetWindowHeight() / 2.0f;
	playRect.width = 200.0f, playRect.height = 125.0f;
	creditsRect.x = CP_System_GetWindowWidth() / 2.0f, creditsRect.y = CP_System_GetWindowHeight() / 1.5f;
	creditsRect.width = 365.0f, creditsRect.height = 120.0f;
	exitRect.x = CP_System_GetWindowWidth() / 2.0f, exitRect.y = CP_System_GetWindowHeight() * 5.0f / 6.0f;
	exitRect.width = 210.0f, exitRect.height = 120.0f;

	// Speed of background measured in how many seconds it takes to cover 1024 pixels.
	backgroundSpeed = 20;

	// Starting height of background 1.
	backgroundHeight1 = 0.0f;

	// Starting height of background 2.
	backgroundHeight2 = 0.0f;

	srand((unsigned int)time(NULL));
}

void Main_Menu_Update(void) {
	CP_Graphics_ClearBackground(white);

	if (playMusicOnce == 0) {
		playMusicOnce = 1;
		CP_Sound_PlayMusic(menuTrack);
	}

	leftMouseClick = CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT);

	deltaTime = CP_System_GetDt();

	// Shifts the background back down seamlessly once its covering the whole screen
	if (backgroundHeight1 < -574) { backgroundHeight1 = 450; }

	backgroundHeight1 = backgroundHeight1 - 1024 / (backgroundSpeed / deltaTime);
	backgroundHeight2 = backgroundHeight1 + 1024;

	CP_Image_Draw(backgroundImage, CP_System_GetWindowWidth() / 2.0f, backgroundHeight1, 2048, 1024, 255);
	CP_Image_Draw(backgroundImage, CP_System_GetWindowWidth() / 2.0f, backgroundHeight2, 2048, 1024, 255);

	// Sets the blocking line to be the same colour as the background.
	CP_Settings_Stroke(brown);

	// Hides the black line connecting the two background images.
	CP_Graphics_DrawLine(0.0f, backgroundHeight1 + 512.0f, 1600.0f, backgroundHeight1 + 512.0f);

	// Title Text
	CP_Settings_TextSize(250.0f);
	titleFont = CP_Font_Load("Assets/Fonts/Frijole-Regular.ttf");
	CP_Font_Set(titleFont);
	
	CP_Settings_Fill(black);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("SinkHole", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() / 4.0f);
	
	// Button Text
	CP_Settings_TextSize(150.0f);
	buttonFont = CP_Font_Load("Assets/Fonts/Margarine-Regular.ttf");
	CP_Font_Set(buttonFont);

	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

	// These have the functionality of isAreaHovered() and not IsAreaClicked()
	if (IsAreaClicked(playRect.x, playRect.y, playRect.width, playRect.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
		// Play Button is hovered
		buttonHovered = 1;
	} else if (IsAreaClicked(exitRect.x, exitRect.y, exitRect.width, exitRect.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
		// Exit Button is hovered
		buttonHovered = 2;
	} else if (IsAreaClicked(creditsRect.x, creditsRect.y, creditsRect.width, creditsRect.height, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
		// Credits Button is hovered
		buttonHovered = 3;
	} else {
		// None is hovered
		buttonHovered = 0;
	}

	// Highlighted Text
	if (buttonHovered == 0) {
		CP_Settings_Fill(lightBrown);
		CP_Font_DrawText("Play", playRect.x, playRect.y - 10.0f);
		CP_Font_DrawText("Credits", creditsRect.x, creditsRect.y);
		CP_Font_DrawText("Quit", exitRect.x, exitRect.y);
	} else if (buttonHovered == 1) {
		CP_Settings_Fill(black);
		CP_Font_DrawText("Play", playRect.x, playRect.y - 10.0f);
		CP_Settings_Fill(lightBrown);
		CP_Font_DrawText("Credits", creditsRect.x, creditsRect.y);
		CP_Font_DrawText("Quit", exitRect.x, exitRect.y);
	} else if (buttonHovered == 2) {
		CP_Settings_Fill(lightBrown);
		CP_Font_DrawText("Play", playRect.x, playRect.y - 10.0f);
		CP_Font_DrawText("Credits", creditsRect.x, creditsRect.y);
		CP_Settings_Fill(black);
		CP_Font_DrawText("Quit", exitRect.x, exitRect.y);
	} else if (buttonHovered == 3) {
		CP_Settings_Fill(lightBrown);
		CP_Font_DrawText("Play", playRect.x, playRect.y - 10.0f);
		CP_Font_DrawText("Quit", exitRect.x, exitRect.y);
		CP_Settings_Fill(black);
		CP_Font_DrawText("Credits", creditsRect.x, creditsRect.y);
	}
	
	// Mouse Input Detection
	if (leftMouseClick && buttonHovered == 1) {
		CP_Sound_StopGroup(CP_SOUND_GROUP_1);
		CP_Engine_SetNextGameState(Game_Init, Game_Update, Game_Exit);
	} else if (leftMouseClick && buttonHovered == 2) {
		CP_Sound_StopGroup(CP_SOUND_GROUP_1);
		CP_Engine_Terminate();
	} else if (leftMouseClick && buttonHovered == 3) {
		CP_Sound_StopGroup(CP_SOUND_GROUP_1);
		CP_Engine_SetNextGameState(Credits_Init, Credits_Update, Credits_Exit);
	}
}

void Main_Menu_Exit(void) {
	CP_Image_Free(&backgroundImage);
	CP_Sound_Free(&menuTrack);
}
