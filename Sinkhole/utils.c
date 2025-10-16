//------------------------------------------------------------------------------------
// file: utils.c
//
// authors:
// l.zheneudamon (primary author)
//		Added Rectangles intersecting 
//		Added Circles and Rectangles intersecting
//		Added CutString and Text Wrap functions
//		Added Animation Loader and unloader
//		Added Animator (Get animation image address)
// timo.duethorn
//		Added Random Number Generator
//		Added is Area Clicked / is Circle Clicked
//		Added are Circles intersecting
//
// brief:
// Contains functions used in other functions of the game.
// These functions help abstract more complicated code and reduce repetitiveness.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

#include <math.h>
#include <stdio.h>
#include <stdlib.h> 
#include "cprocessing.h"
#include "initialize.h"

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y) {
	if (click_x > (area_center_x - (area_width / 2.0f)) && click_x < (area_center_x + (area_width / 2.0f)) && (click_y > area_center_y - (area_height / 2.0f)) && (click_y < area_center_y + (area_height / 2.0f))) {
		return 1;
	} else {
		return 0;
	}
}

int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y) {
	if (sqrtf((circle_center_x - click_x) * (circle_center_x - click_x) + (circle_center_y - click_y) * (circle_center_y - click_y)) < diameter / 2.0f) {
		return 1;
	} else {
		return 0;
	}
}

int AreCirclesIntersecting(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2) {
	if (sqrtf((c1_x - c2_x) * (c1_x - c2_x) + (c1_y - c2_y) * (c1_y - c2_y)) < r1 + r2) {
		return 1;
	} else {
		return 0;
	}
}

int AreRectanglesIntersecting(float r1_x, float r1_y, float r1_width, float r1_height, float r2_x, float r2_y, float r2_width, float r2_height) {
	float leftR1 = r1_x - r1_width / 2.0f;
	float rightR1 = r1_x + r1_width / 2.0f;
	float topR1 = r1_y - r1_height / 2.0f;
	float bottomR1 = r1_y + r1_height / 2.0f;

	float leftR2 = r2_x - r2_width / 2.0f;
	float rightR2 = r2_x + r2_width / 2.0f;
	float topR2 = r2_y - r2_height / 2.0f;
	float bottomR2 = r2_y + r2_height / 2.0f;

	// Check if rectangles do not intersect

	// Check Left Wall of R1 > Right Wall of R2 (R1 is completely on the left of R2, Cannot Intersect)
	// Check Left Wall of R2 > Right Wall of R1 (R1 is completely on the right of R2, Cannot Intersect)
	if (leftR1 > rightR2 || leftR2 > rightR1) {
		return 0;
	}

	// Check Top Wall of R1 > Bottom Wall of R2 (R1 is completely below R2, Cannot Intersect)
	// Check Top Wall of R2 > Bottom Wall of R1 (R2 is completely below R1, Cannot Intersect)
	if (topR1 > bottomR2 || topR2 > bottomR1) {
		return 0;
	}

	return 1;
}

int IsCircleAndRectIntersecting(float circle_x, float circle_y, float circle_diameter, float rect_x, float rect_y, float rect_width, float rect_height) {
	float leftRect = rect_x - rect_width / 2.0f;
	float rightRect = rect_x + rect_width / 2.0f;
	float topRect = rect_y - rect_height / 2.0f;
	float bottomRect = rect_y + rect_height / 2.0f;

	float leftCircle = circle_x - circle_diameter / 2.0f; // Left Most X value of circle
	float rightCircle = circle_x + circle_diameter / 2.0f; // Right Most X value of circle
	float topCircle = circle_y - circle_diameter / 2.0f; // Top Most X value of circle
	float bottomCircle = circle_y + circle_diameter / 2.0f; // Bottom Most X value of circle
	
	// CASE 1: Circle intersected rectangle directly (not through the corners)
	// Check if Circle's X or Y center is inside rect
	
	int circleXInsideRect = (leftRect < circle_x && circle_x < rightRect);
	int circleYInsideRect = (topRect < circle_y && circle_y < bottomRect);

	if (circleXInsideRect) { // CASE 1a: Circle is outside rectangle (determined by Y values)
		if (topRect > bottomCircle || bottomRect < topCircle) {
			return 0;
		} else {
			return 1;
		}
	} else if (circleYInsideRect) { // CASE 1b: Circle is outside rectangle (determined by X values)
		if (leftRect > rightCircle || rightRect < leftCircle) {
			return 0;
		} else {
			return 1;
		}
	} else {
		// CASE 2: Circle intersected rectangle indirectly (hit rectangle corner first)
		// Check if rectangle entered circle's area
		int topLeftRectInCircle = IsCircleClicked(circle_x, circle_y, circle_diameter, leftRect, topRect);
		int topRightRectInCircle = IsCircleClicked(circle_x, circle_y, circle_diameter, rightRect, topRect);
		int bottomLeftRectInCircle = IsCircleClicked(circle_x, circle_y, circle_diameter, leftRect, bottomRect);
		int bottomRightRectInCircle = IsCircleClicked(circle_x, circle_y, circle_diameter, rightRect, bottomRect);

		if (topLeftRectInCircle || topRightRectInCircle || bottomLeftRectInCircle || bottomRightRectInCircle) {
			return 1;
		} else {
			return 0;
		}
	}
}

// Random number function that returns a value within a range specified by the input parameters
int RandomNumber(int min, int max) {
	int value = min + rand() % (max - min + 1);
	return value;
}

void CutString(char string[], int size, int cut) {
	for (int i = 0; i < size-cut; i++) {
		string[i] = string[i + cut];
		string[i + cut] = '\0'; // Emptying String
	}
}

void DrawTextWithWrap(char text[], float x, float y, int charactersPerLine, float gapBetweenEachLine) {
	char textString[MAX_UPGRADE_TEXT] = { 0 };
	sprintf_s(textString, MAX_UPGRADE_TEXT, "%s", text);
	int lastWordPosition = 0, currentLine = 0;
	for (int i = 0; i < MAX_UPGRADE_TEXT; i++) {
		if (textString[i] == ' ') {
			lastWordPosition = i;
		}

		if (i && i % charactersPerLine == 0) {
			textString[lastWordPosition] = '\0';
			CP_Font_DrawText(textString, x, y + currentLine * gapBetweenEachLine);
			CutString(textString, MAX_UPGRADE_TEXT, lastWordPosition + 1);
			i = -1, currentLine++;
			continue;
		}

		if (textString[i] == '\0') {
			break;
		}
	}
	CP_Font_DrawText(textString, x, y + currentLine * gapBetweenEachLine);
}

void AnimationImageLoader(CP_Image imageArray[], char fileName[], int numberOfFrames) {
	for (int i = 0; i < numberOfFrames; ++i) {
		char filePath[MAX_ASSET_FILEPATH] = "Assets/Images/";
		strcat_s(filePath, MAX_ASSET_FILEPATH, fileName);
		char frameNumber[MAX_FRAME_DIGITS] = {0};
		snprintf(frameNumber, MAX_FRAME_DIGITS, "%d", i);
		strcat_s(filePath, MAX_ASSET_FILEPATH, frameNumber);
		strcat_s(filePath, MAX_ASSET_FILEPATH, ".png");
		imageArray[i] = CP_Image_Load(filePath);
	}
}

void ImageUnloader(CP_Image imageArray[], int numberOfImage) {
	for (int i = 0; i < numberOfImage; ++i){
		CP_Image_Free(&imageArray[i]);
	}
}

CP_Image* GetAnimationImageAddress(CP_Image arrayImages[], int MAX_FRAME_IN_ANIMATION, float gameTimer) {
	CP_Image *returningImage = NULL;
	int animationFrame = (int)(gameTimer * MAX_FRAME_IN_ANIMATION) % MAX_FRAME_IN_ANIMATION;
	for (int i = MAX_FRAME_IN_ANIMATION - 1; i >= 0; --i) { // Animate through each frame of the player
		if (animationFrame == i) {
			returningImage = &arrayImages[i];
			break;
		} else {
			returningImage = &arrayImages[0];
		}
	}
	return returningImage;
}
