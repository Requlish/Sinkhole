//------------------------------------------------------------------------------------
// file: credits.c
//
// authors:
// l.zheneudamon (primary author)
//		Added Click to Speed up Credits
//		Added Content of Credits
// timo.duethorn
//		Added Drawing of Background
//		Added Music
//
// brief:
// Contains the main logic of the credits. Contains the game state.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

#include <stdio.h>
#include "cprocessing.h"
#include "mainmenu.h"

CP_Image backgroundImage, copyrightImage;
CP_Sound creditsTrack = NULL;
CP_Font buttonFont, titleFont, textFont;
CP_Color black, brown, lightBrown, white;

struct rect {
	float x;
	float y;
	float height;
	float width;
};
struct rect exitRect;

int leftMouseDown;
int buttonHover = 0;
int backgroundSpeed;
int playMusicOnce;
float deltaTime;
float backgroundHeight1;
float backgroundHeight2;
float ascendedDistance;

void Credits_Init(void) {
	backgroundImage = CP_Image_Load("Assets/Images/Background/background.png");
	copyrightImage = CP_Image_Load("Assets/Images/Others/copyright.png");
	creditsTrack = CP_Sound_LoadMusic("Assets/Music/ancient_ruins.mp3");
	titleFont = CP_Font_Load("Assets/Fonts/RussoOne-Regular.ttf");
	textFont = CP_Font_Load("Assets/Fonts/Alice-Regular.ttf");

	leftMouseDown = 0;
	playMusicOnce = 0;

	black = CP_Color_Create(0, 0, 0, 255);
	brown = CP_Color_Create(65, 40, 4, 255);
	white = CP_Color_Create(255, 255, 255, 255);
	lightBrown = CP_Color_Create(245, 222, 179, 255);

	exitRect.x = CP_System_GetWindowWidth() / 2.0f, exitRect.y = CP_System_GetWindowHeight() * 5.0f / 6.0f;
	exitRect.width = 210.0f, exitRect.height = 120.0f;

	// Speed of background measured in how many seconds it takes to cover 1024 pixels.
	backgroundSpeed = 20;

	// Starting height of background 1.
	backgroundHeight1 = 0.0f;

	// Starting height of background 2.
	backgroundHeight2 = 0.0f;

	// Set increased distance to 0.
	ascendedDistance = 0;
}

void Credits_Update(void) {
	leftMouseDown = CP_Input_MouseDown(MOUSE_BUTTON_LEFT);

	deltaTime = CP_System_GetDt();

	if (playMusicOnce == 0) {
		playMusicOnce = 1;
		CP_Sound_PlayMusic(creditsTrack);
	}

	float ascendSpeed = 1024 / (backgroundSpeed / deltaTime);

	// Mouse Input Detection
	if (CP_Input_KeyTriggered(KEY_ESCAPE)) {
		CP_Sound_StopGroup(CP_SOUND_GROUP_1);
		CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
	} else if (leftMouseDown && ascendedDistance != 3000) {
		ascendSpeed = ascendSpeed * 10;
	}

	if (ascendedDistance < 3000) {
		ascendedDistance += ascendSpeed;
	} else {
		ascendedDistance = 3000;
	}

	// Shifts the background back down seamlessly once its covering the whole screen
	if (backgroundHeight1 < -574) { backgroundHeight1 = 450; }

	backgroundHeight1 = backgroundHeight1 - ascendSpeed;
	backgroundHeight2 = backgroundHeight1 + 1024;

	CP_Graphics_ClearBackground(white);

	CP_Image_Draw(backgroundImage, CP_System_GetWindowWidth() / 2.0f, backgroundHeight1, 2048, 1024, 255);
	CP_Image_Draw(backgroundImage, CP_System_GetWindowWidth() / 2.0f, backgroundHeight2, 2048, 1024, 255);

	// Sets the blocking line to be the same colour as the background.
	CP_Settings_Stroke(brown);

	// Hides the black line connecting the two background images.s
	CP_Graphics_DrawLine(0.0f, backgroundHeight1 + 512.0f, 1600.0f, backgroundHeight1 + 512.0f);

	// Title Text
	CP_Settings_TextSize(250.0f);
	CP_Font_Set(titleFont);

	CP_Settings_Fill(white);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Font_DrawText("Sigma", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - ascendedDistance);
	CP_Settings_TextSize(30.0f);
	CP_Font_Set(textFont);
	CP_Image_Draw(copyrightImage, CP_System_GetWindowWidth() / 2.0f - 340, CP_System_GetWindowHeight() / 2.0f - ascendedDistance, 15, 15, 255);
	CP_Font_DrawText("All Content    2023 DigiPen Institute of Technology Singapore, All Rights Reserved.", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() / 2.0f - ascendedDistance);

	// Developers
	CP_Settings_TextSize(100.0f);
	CP_Font_Set(titleFont);
	CP_Font_DrawText("Developers", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - ascendedDistance + 300);
	CP_Settings_TextSize(30.0f);
	CP_Font_Set(textFont);
	CP_Font_DrawText("Lim Zhen Eu Damon", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - ascendedDistance + 380);
	CP_Font_DrawText("Timo Duethorn", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - ascendedDistance + 420);
	CP_Font_DrawText("Titus Kwong Wen Shuen", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - ascendedDistance + 460);
	CP_Font_DrawText("Loke Kang Wei Matthew", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - ascendedDistance + 500);

	// Instructors
	CP_Settings_TextSize(100.0f);
	CP_Font_Set(titleFont);
	CP_Font_DrawText("Instructors", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - ascendedDistance + 700);
	CP_Settings_TextSize(30.0f);
	CP_Font_Set(textFont);
	CP_Font_DrawText("Cheng Ding Xiang", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - ascendedDistance + 780);
	CP_Font_DrawText("Gerald Wong", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - ascendedDistance + 820);

	// Other Credits Text
	CP_Font_Set(titleFont);
	CP_Settings_TextSize(50.0f);
	CP_Font_DrawText("From", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - ascendedDistance + 1000);
	CP_Settings_TextSize(80.0f);
	CP_Font_DrawText("DigiPen Institute of Technology Singapore", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - ascendedDistance + 1050);
	CP_Settings_TextSize(60.0f);
	CP_Font_DrawText("President", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - ascendedDistance + 1250);
	CP_Settings_TextSize(30.0f);
	CP_Font_Set(textFont);
	CP_Font_DrawText("Claude Comair", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - ascendedDistance + 1300);

	CP_Font_Set(titleFont);
	CP_Settings_TextSize(60.0f);
	CP_Font_DrawText("Executives", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - ascendedDistance + 1400);
	CP_Settings_TextSize(30.0f);
	CP_Font_Set(textFont);
	CP_Font_DrawText("Jason Chu | Christopher Comair | Micheal Gats", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - ascendedDistance + 1450);
	CP_Font_DrawText("Michele Comair | Raymond Yan | Samir Abou Samra", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - ascendedDistance + 1500);
	CP_Font_DrawText("Prasanna Ghali | John Bauer | Dr. Erik Mohrmann", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - ascendedDistance + 1550);
	CP_Font_DrawText("Melvin Gonsalvez | Anglea Kugler | Dr. Charles Duba", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - ascendedDistance + 1600);
	CP_Font_DrawText("Ben Ellinger | Johnny Deek", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - ascendedDistance + 1650);
	CP_Image_Draw(copyrightImage, CP_System_GetWindowWidth() / 2.0f - 340, CP_System_GetWindowHeight() - ascendedDistance + 1800, 15, 15, 255);
	CP_Font_DrawText("All Content    2023 DigiPen Institute of Technology Singapore, All Rights Reserved.", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - ascendedDistance + 1800);

	CP_Font_Set(titleFont);
	CP_Settings_TextSize(200.0f);
	CP_Font_DrawText("Thank You!", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - ascendedDistance + 2500);
	CP_Settings_TextSize(25.0f);
	CP_Font_Set(textFont);
	CP_Font_DrawText("Press ESC to Exit", CP_System_GetWindowWidth() / 2.0f, CP_System_GetWindowHeight() - ascendedDistance + 2900);
}

void Credits_Exit(void) {
	CP_Image_Free(&backgroundImage);
	CP_Image_Free(&copyrightImage);
	CP_Sound_Free(&creditsTrack);
}
