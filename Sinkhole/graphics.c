//------------------------------------------------------------------------------------
// file: graphics.c
//
// authors:
// l.zheneudamon (primary author)
//		Added Health UI
//		Added Upgrade related functions
//		Added Health Pickups
//		Added Lazer Hazards
//		Added Damaged View
// timo.duethorn
//		Added drawing of Platforms
//		Added drawing of Pause Menu
//		Added drawing of Defeat Text
// tituswenshuen.kwong
//		Flying Enemy drawing
//		Bullet Drawing
// l.kangweimatthew
//		Walking Enemy drawing
//
// brief:
// Contains most of the draw functions of the game.
// Also handles animation for those required.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

#include <math.h>
#include <stdio.h>
#include <string.h>
#include "cprocessing.h"
#include "initialize.h"
#include "utils.h"

void DrawHeatlhUI(CP_Image healthEmptyUI, CP_Image healthFullUI, int playerCurrentHealth, int playerMaxHealth, float imageSize, float startingYHeight, float wallWidth) {
	// 0. Adjust height to size
	startingYHeight -= imageSize / 2.0f;

	// 1. First get how many hearts can fit per row
	int heartsPerRow = 0;
	for (float i = imageSize; i < wallWidth; i += imageSize) {
		heartsPerRow++;
	}

	// 2. Get gap between Wall and Heart
	float startingGap = (wallWidth - (heartsPerRow * imageSize) + imageSize) / 2.0f;

	// 3. Create nested for-loops using Rows and number of hearts
	int numberOfRows = (playerMaxHealth / heartsPerRow) + 1;
	for (int i = 0; i < numberOfRows; ++i) {
		for (int j = 0; (playerMaxHealth != 0 && j < heartsPerRow); ++j) { // Don't draw more hearts than my maximum
			if (playerCurrentHealth <= 0) {
				// Draw Empty Hearts
				CP_Image_Draw(healthEmptyUI, startingGap + (j * imageSize), startingYHeight, imageSize, imageSize, 255);
				playerMaxHealth--;
			} else {
				// Draw Full Hearts
				CP_Image_Draw(healthFullUI, startingGap + (j * imageSize), startingYHeight, imageSize, imageSize, 255);
				playerMaxHealth--;
				playerCurrentHealth--;
			}
		}
		startingYHeight += imageSize;
	}
}

void DrawUpgradeSelection(
	CP_Image upgradeIcon1, char upgradeName1[], char upgradeText1[],
	CP_Image upgradeIcon2, char upgradeName2[], char upgradeText2[],
	CP_Image upgradeIcon3, char upgradeName3[], char upgradeText3[],
	CP_Font upgradeFont, float maxY, float animationTime, float fadeTime, int selectedUpgrade
) {
	CP_Color fadeInSilver, silverSelected;
	float fadeInTime = animationTime / fadeTime > 1 ? 1: animationTime / fadeTime;
	int fadeInAlpha = (int)(255 * fadeInTime);
	fadeInSilver = CP_Color_Create(192, 192, 192, fadeInAlpha);
	silverSelected = CP_Color_Create(224, 224, 224, 255);
	CP_Settings_Fill(fadeInSilver);

	float upgrade1X = CP_System_GetWindowWidth() * 0.25f;
	float upgrade2X = CP_System_GetWindowWidth() * 0.50f;
	float upgrade3X = CP_System_GetWindowWidth() * 0.75f;

	// Draws Borders
	CP_Graphics_DrawRect(upgrade1X, maxY, CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.8f);
	CP_Graphics_DrawRect(upgrade2X, maxY, CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.8f);
	CP_Graphics_DrawRect(upgrade3X, maxY, CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.8f);

	// Draws Icon of Upgrades
	float iconSize = 150.0f;
	if (fadeInTime == 1){
		CP_Settings_Fill(silverSelected);
		if (selectedUpgrade == 1) {
			CP_Graphics_DrawRect(upgrade1X, maxY, CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.8f);
		} else if (selectedUpgrade == 2) {
			CP_Graphics_DrawRect(upgrade2X, maxY, CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.8f);
		} else if (selectedUpgrade == 3) {
			CP_Graphics_DrawRect(upgrade3X, maxY, CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.8f);
		}

		// Icons
		CP_Image_Draw(upgradeIcon1, upgrade1X, maxY - CP_System_GetWindowHeight() * 0.25f, iconSize, iconSize, 255);
		CP_Image_Draw(upgradeIcon2, upgrade2X, maxY - CP_System_GetWindowHeight() * 0.25f, iconSize, iconSize, 255);
		CP_Image_Draw(upgradeIcon3, upgrade3X, maxY - CP_System_GetWindowHeight() * 0.25f, iconSize, iconSize, 255);

		// Title
		CP_Settings_TextSize(50.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
		CP_Font_Set(upgradeFont);

		DrawTextWithWrap(upgradeName1, upgrade1X, maxY - CP_System_GetWindowHeight() * 0.1f, 15, 60);
		DrawTextWithWrap(upgradeName2, upgrade2X, maxY - CP_System_GetWindowHeight() * 0.1f, 15, 60);
		DrawTextWithWrap(upgradeName3, upgrade3X, maxY - CP_System_GetWindowHeight() * 0.1f, 15, 60);

		// Text
		CP_Settings_TextSize(30.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
		CP_Font_Set(upgradeFont);
		
		DrawTextWithWrap(upgradeText1, upgrade1X, maxY + CP_System_GetWindowHeight() * 0.1f, 25, 50);
		DrawTextWithWrap(upgradeText2, upgrade2X, maxY + CP_System_GetWindowHeight() * 0.1f, 25, 50);
		DrawTextWithWrap(upgradeText3, upgrade3X, maxY + CP_System_GetWindowHeight() * 0.1f, 25, 50);
	}
}

void DrawPlatforms(struct Platform platforms[],int drawnPlatforms, float platformHeight) {
	CP_Color black = CP_Color_Create(0, 0, 0, 255);
	CP_Color grey = CP_Color_Create(99, 99, 99, 255);
	CP_Settings_Stroke(black);
	CP_Settings_Fill(grey);

	// Cycle through platforms array to draw all platforms
	for (int i = 0; i < drawnPlatforms; ++i) {
		if (platforms[i].start == 0) { // Start with Platform
			CP_Graphics_DrawRect(platforms[i].p1Coord, platforms[i].y * 100.0f, (float)platforms[i].p1Length, platformHeight);
			if (platforms[i].p2Length) {
				CP_Graphics_DrawRect(platforms[i].p2Coord, platforms[i].y * 100.0f, (float)platforms[i].p2Length, platformHeight);
			}
		} else if (platforms[i].start == 1) { // Start with Gap
			CP_Graphics_DrawRect(platforms[i].p1Coord, platforms[i].y * 100.0f, (float)platforms[i].p1Length, platformHeight);
			if (platforms[i].p2Length) {
				CP_Graphics_DrawRect(platforms[i].p2Coord, platforms[i].y * 100.0f, (float)platforms[i].p2Length, platformHeight);
			}
		}
	}
}

void DrawAfterUpgradeSelection(CP_Image upgradeIcon, char upgradeName[], char upgradeText[], CP_Font upgradeFont, float maxY, float animationTime, float fadeTime, int selectedUpgrade) {
	CP_Color fadeOutSilver, silverSelected;
	float fadeInTime = animationTime / fadeTime > 1 ? 1 : animationTime / fadeTime;
	int fadeInAlpha = (int)(255 - (255 * fadeInTime));
	fadeOutSilver = CP_Color_Create(192, 192, 192, fadeInAlpha);
	silverSelected = CP_Color_Create(224, 224, 224, 255);
	CP_Settings_Fill(fadeOutSilver);

	float upgradeX;

	if (selectedUpgrade == 1) {
		upgradeX = CP_System_GetWindowWidth() * 0.25f;
	} else if (selectedUpgrade == 2) {
		upgradeX = CP_System_GetWindowWidth() * 0.50f;
	} else {
		upgradeX = CP_System_GetWindowWidth() * 0.75f;
	}

	float upgrade1X = CP_System_GetWindowWidth() * 0.25f;
	float upgrade2X = CP_System_GetWindowWidth() * 0.50f;
	float upgrade3X = CP_System_GetWindowWidth() * 0.75f;

	// Draws Borders
	CP_Graphics_DrawRect(upgrade1X, maxY, CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.8f);
	CP_Graphics_DrawRect(upgrade2X, maxY, CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.8f);
	CP_Graphics_DrawRect(upgrade3X, maxY, CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.8f);

	// Draws Icon of Upgrades
	float iconSize = 150.0f;
	CP_Settings_Fill(silverSelected);
	if (selectedUpgrade == 1) {
		CP_Graphics_DrawRect(upgrade1X, maxY, CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.8f);
	} else if (selectedUpgrade == 2) {
		CP_Graphics_DrawRect(upgrade2X, maxY, CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.8f);
	} else if (selectedUpgrade == 3) {
		CP_Graphics_DrawRect(upgrade3X, maxY, CP_System_GetWindowWidth() * 0.2f, CP_System_GetWindowHeight() * 0.8f);
	}

	// Icons
	CP_Image_Draw(upgradeIcon, upgradeX, maxY - CP_System_GetWindowHeight() * 0.25f, iconSize, iconSize, 255);

	// Title
	CP_Settings_TextSize(50.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Font_Set(upgradeFont);

	DrawTextWithWrap(upgradeName, upgradeX, maxY - CP_System_GetWindowHeight() * 0.1f, 15, 60);

	// Text
	CP_Settings_TextSize(30.0f);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Font_Set(upgradeFont);

	DrawTextWithWrap(upgradeText, upgradeX, maxY + CP_System_GetWindowHeight() * 0.1f, 25, 50);
}

void DrawUpgradeUI(CP_Image upgradeIcon[], int playerUpgrades[], float imageSize, float startingYHeight, float wallWidth) {
	// 0. Adjust height to size
	startingYHeight -= imageSize / 2.0f;

	// 0. Create Array to draw only Upgrades Gotten
	int upgradesAvailable[MAX_UPGRADES] = { 0 }; // "0"s in upgradesAvailable will be ignored.
	int counter = 0;
	for (int i = 0; i < MAX_UPGRADES; i++) {
		if (playerUpgrades[i] != 0) {
			upgradesAvailable[counter] = i + 1; // Upgrade Number 1 will be 1 in upgradesAvailable. 
			counter++;
		}
	}

	// 1. First get how many upgrades can fit per row
	int upgradesPerRow = 0;
	for (float i = imageSize; i < wallWidth; i += imageSize) {
		upgradesPerRow++;
	}

	// 2. Get gap between Wall and Upgrades
	float startingGap = (wallWidth - (upgradesPerRow * imageSize) + imageSize) / 2.0f;

	// 3. Create nested for-loops using Rows and number of upgrades
	int numberOfUpgradesDrawn = 0;
	int numberOfRows = (counter / upgradesPerRow) + 1;
	for (int i = 0; i < numberOfRows; ++i) {
		for (int j = 0; (counter != 0 && j < upgradesPerRow); ++j) { // Don't draw more upgrades than my maximum
			int elementOfImage = upgradesAvailable[numberOfUpgradesDrawn] - 1;
			CP_Image_Draw(upgradeIcon[elementOfImage], startingGap + (j * imageSize), startingYHeight, imageSize, imageSize, 255);
			// Counter for repeatable upgrades
			if (elementOfImage < 3) {
				float offset = imageSize * 3.0f / 8.0f;
				CP_Font exclaimFont = CP_Font_Load("Assets/Fonts/Exo2-Regular.ttf");
				CP_Settings_TextSize(20.0f);
				CP_Font_Set(exclaimFont);
				CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
				char number = playerUpgrades[elementOfImage] + '0';
				char myUpgradeCount[2] = {number, '\0'};
				CP_Font_DrawText(myUpgradeCount, startingGap + (j * imageSize) + offset, startingYHeight + offset); // Offset to match center of triangle
			}
			numberOfUpgradesDrawn++;
			counter--;
		}
		startingYHeight += imageSize;
	}
}

void DrawPauseMenu(float maxY, int selectedButton) {
	float pauseTextX = CP_System_GetWindowWidth() / 2.0f, pauseTextY = maxY - 200;
	char pauseText[] = "Paused";

	float resumeButtonX = CP_System_GetWindowWidth() / 2.0f, resumeButtonY = maxY, resumeButtonWidth = 600, resumeButtonHeight = 100;
	char resumeText[] = "Resume";

	float mainmenuButtonX = CP_System_GetWindowWidth() / 2.0f, mainmenuButtonY = maxY + 100, mainmenuButtonWidth = 600, mainmenuButtonHeight = 100;
	char mainmenuText[] = "Main Menu";

	float exitButtonX = CP_System_GetWindowWidth() / 2.0f, exitButtonY = maxY + 200, exitButtonWidth = 600, exitButtonHeight = 100;
	char exitText[] = "Quit";

	CP_Color black = CP_Color_Create(0, 0, 0, 255);
	CP_Color lightBrown = CP_Color_Create(245, 222, 179, 255);

	CP_Font buttonsFont = CP_Font_Load("Assets/Fonts/Margarine-Regular.ttf");

	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

	CP_Settings_TextSize(300.0f);
	CP_Font_Set(buttonsFont);
	CP_Settings_Fill(black);
	CP_Font_DrawText(pauseText, pauseTextX, pauseTextY);
	
	CP_Settings_TextSize(100.0f);
	if (selectedButton == 0) {
		CP_Settings_Fill(lightBrown);
		CP_Font_DrawText(resumeText, resumeButtonX, resumeButtonY);
		CP_Font_DrawText(mainmenuText, mainmenuButtonX, mainmenuButtonY);
		CP_Font_DrawText(exitText, exitButtonX, exitButtonY);
	} else if (selectedButton == 1) {
		CP_Settings_Fill(black);
		CP_Font_DrawText(resumeText, resumeButtonX, resumeButtonY);
		CP_Settings_Fill(lightBrown);
		CP_Font_DrawText(mainmenuText, mainmenuButtonX, mainmenuButtonY);
		CP_Font_DrawText(exitText, exitButtonX, exitButtonY);
	} else if (selectedButton == 2) {
		CP_Settings_Fill(lightBrown);
		CP_Font_DrawText(resumeText, resumeButtonX, resumeButtonY);
		CP_Font_DrawText(exitText, exitButtonX, exitButtonY);
		CP_Settings_Fill(black);
		CP_Font_DrawText(mainmenuText, mainmenuButtonX, mainmenuButtonY);
	} else {
		CP_Settings_Fill(lightBrown);
		CP_Font_DrawText(resumeText, resumeButtonX, resumeButtonY);
		CP_Font_DrawText(mainmenuText, mainmenuButtonX, mainmenuButtonY);
		CP_Settings_Fill(black);
		CP_Font_DrawText(exitText, exitButtonX, exitButtonY);
	}
}

void DrawDefeatText(float maxY, float scoreMultiplier, int selectedButton) {
	float defeatTextX = CP_System_GetWindowWidth() / 2.0f, defeatTextY = maxY - 200;
	char defeatText[] = "YOU DIED";

	float scoreButtonX = CP_System_GetWindowWidth() / 2.0f, scoreButtonY = maxY, scoreButtonWidth = 600, scoreButtonHeight = 100;
	char scoreText[7 + MAX_SCORE_DIGITS] = { 0 }; // 7 chars for "SCORE: "
	int scoreSize = 7 + MAX_SCORE_DIGITS;
	float highscoreInteger = (maxY - 347) * scoreMultiplier;
	snprintf(scoreText, scoreSize, "SCORE: %d", (int)highscoreInteger);

	float respawnButtonX = CP_System_GetWindowWidth() / 2.0f, respawnButtonY = maxY + 100, respawnButtonWidth = 600, respawnButtonHeight = 100;
	char respawnText[] = "Respawn";

	float mainmenuButtonX = CP_System_GetWindowWidth() / 2.0f, mainmenuButtonY = maxY + 200, mainmenuButtonWidth = 600, mainmenuButtonHeight = 100;
	char mainmenuText[] = "Main Menu";

	CP_Color white = CP_Color_Create(255, 255, 255, 255);
	CP_Color darkRed = CP_Color_Create(130, 16, 29, 255);
	CP_Color lightBrown = CP_Color_Create(245, 222, 179, 255);

	CP_Font buttonFont = CP_Font_Load("Assets/Fonts/Margarine-Regular.ttf");
	CP_Font defeatFont = CP_Font_Load("Assets/Fonts/IMFeENsc28P.ttf");
	CP_Font scoreFont = CP_Font_Load("Assets/Fonts/PressStart2P-Regular.ttf");

	CP_Graphics_ClearBackground(white);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);

	// YOU DIED
	CP_Settings_TextSize(300.0f);
	CP_Font_Set(defeatFont);
	CP_Settings_Fill(darkRed);
	CP_Font_DrawText(defeatText, defeatTextX, defeatTextY);

	// SCORE
	CP_Settings_TextSize(50.0f);
	CP_Font_Set(scoreFont);
	CP_Settings_Fill(lightBrown);
	CP_Font_DrawText(scoreText, scoreButtonX, scoreButtonY);

	// MENU BUTTONS
	CP_Settings_TextSize(100.0f);
	CP_Font_Set(buttonFont);
	if (selectedButton == 0) {
		CP_Settings_Fill(lightBrown);
		CP_Font_DrawText(respawnText, respawnButtonX, respawnButtonY);
		CP_Font_DrawText(mainmenuText, mainmenuButtonX, mainmenuButtonY);
	} else if (selectedButton == 1) {
		CP_Settings_Fill(darkRed);
		CP_Font_DrawText(respawnText, respawnButtonX, respawnButtonY);
		CP_Settings_Fill(lightBrown);
		CP_Font_DrawText(mainmenuText, mainmenuButtonX, mainmenuButtonY);
	} else {
		CP_Settings_Fill(lightBrown);
		CP_Font_DrawText(respawnText, respawnButtonX, respawnButtonY);
		CP_Settings_Fill(darkRed);
		CP_Font_DrawText(mainmenuText, mainmenuButtonX, mainmenuButtonY);
	}
}

void DrawPickupHealth(struct Pickups pickups[]) {
	for (int i = 0; i < MAX_HEALTH_PICKUPS; ++i) {
		if (pickups[i].isActive) { // &pickup[i].imageAddress
			CP_Image_Draw(*pickups[i].imageAddress, pickups[i].x, pickups[i].y, pickups[i].imageSize, pickups[i].imageSize, 255);
		}
	}
}

void DrawBullets(struct Bullet bullets[], CP_Image bulletImage) {
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].isActive) {
			CP_Image_Draw(bulletImage, bullets[i].x, bullets[i].y, bullets[i].diameterSize, bullets[i].diameterSize, 255);
		}
	}
}

void DrawFlyingEnemies(struct Flying_Enemy enemies[], struct Player player, CP_Image enemyLeft[], CP_Image enemyRight[], float gameTimer) {
	for (int i = 0; i < MAX_FLYING_ENEMIES; i++) {
		if (enemies[i].isActive) {
			if (enemies[i].x < player.x) { // Right
				enemies[i].imageAddress = GetAnimationImageAddress(enemyRight, MAX_FRAME_ENEMY_FLYING_RIGHT, gameTimer);
			} else { // Left
				enemies[i].imageAddress = GetAnimationImageAddress(enemyLeft, MAX_FRAME_ENEMY_FLYING_LEFT, gameTimer);
			}
			CP_Image_Draw(*enemies[i].imageAddress, enemies[i].x, enemies[i].y, enemies[i].diameter, enemies[i].diameter, 255);
		}
	}
}

void DrawLazerHazard(struct Lazer_Hazard lazerHazard[], float gameTimer, float wallWidth) {
	for (int i = 0; i < MAX_LAZER_HAZARDS; ++i) {
		if (lazerHazard[i].isActive) {
			// Following the sequence of "blink, blink, zap"
			// Will show yellow twice, before showing orange
			float animationMultiplier = lazerHazard[i].timeToFire / 3.0f; // Determines how long the lazer blinks for
			float animationTimer = gameTimer - lazerHazard[i].timeCreated; // Time from 0 up to how long the lazer blinks for
			for (int j = 0; j < 2; ++j) { // Because of "blink, blink, zap". Only 2 times
				if (animationTimer > animationMultiplier) {
					animationTimer -= animationMultiplier;
				} else {
					break;
				}
			}
			int yellowAlpha = (int)(50 * animationTimer);
			int orangeAlpha = (int)(255 * animationTimer);
			CP_Color yellow = CP_Color_Create(242, 242, 23, yellowAlpha);
			CP_Color orange = CP_Color_Create(255, 102, 0, orangeAlpha);
			float growingHeight = lazerHazard[i].height * animationTimer;
			if (gameTimer > lazerHazard[i].timeCreated + (lazerHazard[i].timeToFire * 2.0f / 3.0f)) { // Shoot Orange Lazer
				CP_Settings_Fill(orange);
				CP_Graphics_DrawRect(CP_System_GetWindowWidth() / 2.0f, lazerHazard[i].y, CP_System_GetWindowWidth() - 2 * wallWidth, growingHeight);
			} else {
				CP_Settings_Fill(yellow);
				CP_Graphics_DrawRect(CP_System_GetWindowWidth() / 2.0f, lazerHazard[i].y, CP_System_GetWindowWidth() - 2 * wallWidth, growingHeight);
			}
			// Let 25 be offset from Left Side of Right Side Wall
			// Triangle will span 100 x 100. X value of triangle center will be 25 + 50
			float warningTriangleX = CP_System_GetWindowWidth() - wallWidth + 75;
			float warningTriangleY = lazerHazard[i].y;
			float trianglePoint1X = warningTriangleX - 50, trianglePoint1Y = warningTriangleY + 50;
			float trianglePoint2X = warningTriangleX, trianglePoint2Y = warningTriangleY - 50;
			float trianglePoint3X = warningTriangleX + 50, trianglePoint3Y = warningTriangleY + 50;
			CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255));
			CP_Graphics_DrawTriangle(trianglePoint1X, trianglePoint1Y, trianglePoint2X, trianglePoint2Y, trianglePoint3X, trianglePoint3Y);
			CP_Font exclaimFont = CP_Font_Load("Assets/Fonts/Exo2-Regular.ttf");
			CP_Settings_TextSize(120.0f);
			CP_Font_Set(exclaimFont);
			CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
			CP_Font_DrawText("!", warningTriangleX + 14, warningTriangleY + 5); // Offset to match center of triangle
		}
	}
}

void DrawWalkingEnemies(struct Walking_Enemy enemies[], CP_Image enemyLeft[], CP_Image enemyRight[], float gameTimer) {
	for (int i = 0; i < MAX_WALKING_ENEMIES; ++i) {
		if (enemies[i].isActive) {
			if (enemies[i].dir) { // Right
				enemies[i].imageAddress = GetAnimationImageAddress(enemyRight, MAX_FRAME_ENEMY_WALKING_RIGHT, gameTimer);
			} else { // Left
				enemies[i].imageAddress = GetAnimationImageAddress(enemyLeft, MAX_FRAME_ENEMY_WALKING_LEFT, gameTimer);
			}
			CP_Image_Draw(*enemies[i].imageAddress, enemies[i].x, enemies[i].y, (float)enemies[i].width, (float)enemies[i].height, 255);
		}
	}
}

void DrawDamagedView(struct Player player, float maxY, CP_Image damagedScreen) {
	float opacityScale = 1 - (float)player.currentHealth / (float)player.maxHealth;
	int opacityAlpha = (int)(50 * opacityScale);
	CP_Image_Draw(damagedScreen, CP_System_GetWindowWidth() / 2.0f, maxY, CP_System_GetWindowWidth() + 50.0f, CP_System_GetWindowHeight() + 50.0f, opacityAlpha);
}
