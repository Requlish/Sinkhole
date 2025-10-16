//------------------------------------------------------------------------------------
// file: mainmenu.c
//
// authors:
// timo.duethorn (primary author)
//		Added function to randomly generate a new platform
//		Added function to shift platforms through an array and generate a new platform at the end of the array
// 
// brief:
// Contains platform related functions.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

#include "stdio.h" // For printf
#include "cprocessing.h"
#include "initialize.h"
#include "utils.h"

void GeneratePlatform(struct Player player, struct Platform platforms[], int i, float wallWidth) {
	int startValue = RandomNumber(0, 1);

	if (platforms[i].y > 20 + (50 * player.totalUpgrades) && platforms[i].y < 30 + (50 * player.totalUpgrades)) {
		platforms[i].start = 2;
	} else {
		platforms[i].start = startValue;
	}

	int playableSpace = 960;
	int minPlatform = 250;
	int minGap = 175;

	if (platforms[i].start == 0) { // Start with Platform
		int remainder = playableSpace;
		int platformWidth1 = RandomNumber(minPlatform, remainder - minGap);
		platforms[i].p1Length = platformWidth1;
		platforms[i].p1Coord = platformWidth1 / 2.0f + wallWidth;
		remainder = remainder - platformWidth1;

		if (remainder < minPlatform + minGap) { // 1 Platform, 1 Gap
			platforms[i].g1Length = remainder;
			platforms[i].p2Length = 0;
			platforms[i].p2Coord = (float)CP_System_GetWindowWidth();
			platforms[i].g2Length = 0;
		} else if (remainder > 0) { // 1 Platform, 1 Gap & more
			int gapWidth1 = RandomNumber(minGap, remainder - minPlatform);
			platforms[i].g1Length = gapWidth1;
			remainder = remainder - gapWidth1;

			if (remainder < minPlatform + minGap) { // 1 Platform, 1 Gap, 1 Platform
				platforms[i].p2Length = remainder;
				platforms[i].p2Coord = remainder / 2.0f + wallWidth + platformWidth1 + gapWidth1;
				platforms[i].g2Length = 0;
			} else if (remainder > 0) { // 1 Platform, 1 Gap, 1 Platform, 1 Gap
				int platformWidth2 = RandomNumber(minPlatform, remainder - minGap);
				platforms[i].p2Length = platformWidth2;
				platforms[i].p2Coord = platformWidth2 / 2.0f + wallWidth + platformWidth1 + gapWidth1;
				remainder = remainder - platformWidth2;

				platforms[i].g2Length = remainder;
			}
		}
	} else if (platforms[i].start == 1) { // Start with Gap
		int remainder = playableSpace;
		int gapWidth1 = RandomNumber(minGap, remainder - minPlatform);
		platforms[i].g1Length = gapWidth1;
		remainder = remainder - gapWidth1;

		if (remainder < minGap + minPlatform) { // 1 Gap, 1 Platform
			platforms[i].p1Length = remainder;
			platforms[i].p1Coord = remainder / 2.0f + wallWidth + gapWidth1;
			platforms[i].p2Length = 0;
			platforms[i].p2Coord = (float)CP_System_GetWindowWidth();
			platforms[i].g2Length = 0;
		} else if (remainder > 0) { // 1 Gap, 1 Platform & more
			int platformWidth1 = RandomNumber(minPlatform, remainder - minGap);
			platforms[i].p1Length = platformWidth1;
			platforms[i].p1Coord = platformWidth1 / 2.0f + wallWidth + gapWidth1;
			remainder = remainder - platformWidth1;

			if (remainder < minGap + minPlatform) { // 1 Gap, 1 Platform, 1 Gap
				platforms[i].g2Length = remainder;
				platforms[i].p2Length = 0;
				platforms[i].p2Coord = (float)CP_System_GetWindowWidth();
			} else if (remainder > 0) { // 1 Gap, 1 Platform, 1 Gap, 1 Platform
				int gapWidth2 = RandomNumber(minGap, remainder - minPlatform);
				platforms[i].g2Length = gapWidth2;
				remainder = remainder - gapWidth2;

				platforms[i].p2Length = remainder;
				platforms[i].p2Coord = remainder / 2.0f + wallWidth + gapWidth1 + platformWidth1 + gapWidth2;
			}
		}
	} else if (platforms[i].start == 2) { // platforms data = 0; Generate Empty Layer / No Platforms
		platforms[i].g1Length = 0;
		platforms[i].p1Length = 0;
		platforms[i].p1Coord = 0.0;
		platforms[i].g2Length = 0;
		platforms[i].p2Length = 0;
		platforms[i].p2Coord = (float)CP_System_GetWindowWidth();
	}

	if (platforms[i].y == 5) { // First Platform Data
		platforms[i].start = 1;
		platforms[i].g1Length = 300;
		platforms[i].p1Length = 360;
		platforms[i].p1Coord = 800.0;
		platforms[i].g2Length = 300;
		platforms[i].p2Length = 0;
		platforms[i].p2Coord = (float)CP_System_GetWindowWidth();
	}

	if (platforms[i].y == 7) { // Second Platform Data
		platforms[i].start = 0;
		platforms[i].g1Length = 360;
		platforms[i].p1Length = 300;
		platforms[i].p1Coord = 470.0;
		platforms[i].g2Length = 0;
		platforms[i].p2Length = 300;
		platforms[i].p2Coord = 1130.0;
	}
}

void ShiftPlatform(struct Player player, struct Platform platforms[], int drawnPlatforms, int newY, float wallWidth) {
	for (int i = 0, j = 1; i < drawnPlatforms; ++i, ++j) {
		if (i != (drawnPlatforms - 1)) {
			platforms[i] = platforms[j];
		} else {
			platforms[i].y = newY;
			GeneratePlatform(player, platforms, i, wallWidth);
		}
	}
}
