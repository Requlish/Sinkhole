//------------------------------------------------------------------------------------
// file: hazards.c
//
// authors:
// l.zheneudamon (primary author)
//		Added all hazards functions
//
// brief:
// Contains functions used for hazards.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

#include "cprocessing.h"
#include "initialize.h"
#include "utils.h"

void SpawnLazerHazard(struct Lazer_Hazard lazerHazard[], float timeToFire, float y, float height, float gameTime) {
	for (int i = 0; i < MAX_LAZER_HAZARDS; ++i) {
		if (!lazerHazard[i].isActive) {
			lazerHazard[i].isActive = 1;
			lazerHazard[i].y = y;
			lazerHazard[i].height = height;
			lazerHazard[i].timeToFire = timeToFire;
			lazerHazard[i].timeCreated = gameTime;
			break;
		}
	}
}

int DidLazerHitPlayer(struct Player *player, struct Lazer_Hazard *lazerHazard) { // Run this when lazer timer run out
	if (AreRectanglesIntersecting(player->x, player->y, player->width, player->height, CP_System_GetWindowWidth() / 2.0f, lazerHazard->y, (float)CP_System_GetWindowWidth(), lazerHazard->height)) {
		lazerHazard->isActive = 0;
		player->currentHealth -= 1;
		return 1;
	}
	lazerHazard->isActive = 0;
	return 0;
}

void DespawnLazerHazard(struct Lazer_Hazard lazerHazard[], float maxY) { // Run this every frame to remove hazards outside screen
	for (int i = 0; i < MAX_LAZER_HAZARDS; ++i) {
		if (lazerHazard[i].isActive && lazerHazard[i].y < maxY - CP_System_GetWindowHeight()) {
			lazerHazard[i].isActive = 0;
		}
	}
}