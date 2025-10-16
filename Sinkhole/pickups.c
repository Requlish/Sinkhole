//------------------------------------------------------------------------------------
// file: pickups.c
//
// authors:
// l.zheneudamon (primary author)
//		Added all pickups menu functions
//
// brief:
// Contains functions used for pickups.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

#include "cprocessing.h"
#include "initialize.h"
#include "utils.h"

void SpawnHealthPickup(struct Pickups pickups[], float pickupX, float pickupY, int percent) {
	int rng = RandomNumber(0, 99);
	if (rng < percent) {
		for (int i = 0; i < MAX_HEALTH_PICKUPS; ++i) {
			if (!pickups[i].isActive) {
				pickups[i].isActive = 1;
				pickups[i].x = pickupX;
				pickups[i].y = pickupY;
				pickups[i].imageSize = PICKUP_SIZE;
				break;
			}
		}
	}
}

int CheckPlayerPickedUpHealth(struct Player player, struct Pickups pickups[]) { // Returns 1 if Player gain health
	for (int i = 0; i < MAX_HEALTH_PICKUPS; ++i) {
		if (pickups[i].isActive && AreRectanglesIntersecting(player.x, player.y, player.width, player.height, pickups[i].x, pickups[i].y, pickups[i].imageSize, pickups[i].imageSize)) {
			if (player.currentHealth != player.maxHealth){
				pickups[i].isActive = 0;
				return 1; // Bug/Feature: Only 1 pickup per frame
			}
		}
	}
	return 0;
}

void DespawnHealthPickup(struct Pickups pickups[], float maxY) {
	for (int i = 0; i < MAX_HEALTH_PICKUPS; ++i) {
		if (pickups[i].isActive && pickups[i].y < maxY - CP_System_GetWindowHeight()) {
			pickups[i].isActive = 0;
		}
	}
}

void AnimatePickupHealths(struct Pickups pickups[], CP_Image pickupsHealth[], float gameTimer) {
	for (int i = 0; i < MAX_HEALTH_PICKUPS; ++i) {
		if (pickups[i].isActive) {
			pickups[i].imageAddress = GetAnimationImageAddress(pickupsHealth, MAX_FRAME_PICKUP_HEALTH, gameTimer);
		}
	}
}
