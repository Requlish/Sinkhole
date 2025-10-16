//------------------------------------------------------------------------------------
// file: physics.c
//
// authors:
// l.kangweimatthew (primary author)
//		Added collision for game boundaries (Right/Left Wall Collision & Ceiling Collision)
// l.zheneudamon 
//		Added the IsPlayerStopped function to detect case of player inside the platform in the next frame
//
// brief:
// Contains the physics functions definitions that are used in the game to detect collision.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

#include <math.h>
#include <stdio.h>
#include "cprocessing.h"
#include "initialize.h"

int IsRightWallCollided(float playerX, float playerWidth, float wallWidth) {
	float rightWall = CP_System_GetWindowWidth() - wallWidth;
	if (playerX + playerWidth / 2 > rightWall) {
		return 1;
	}
	return 0;
}

int IsLeftWallCollided(float playerX, float playerWidth, float wallWidth) {
	if (playerX - playerWidth/2 < wallWidth) {
		return 1;
	}
	return 0;
}

int IsCeilingCollided(float playerY, float playerHeight, float maxY) {
	maxY -= CP_System_GetWindowHeight() / 2.0f;
	if (playerY - playerHeight / 2 < maxY) {
		return 1;
	}
	return 0;
}

int IsPlayerStopped(struct Player player, int scenario, float platfromCornerX, float platformCornerY) {
	// This function should ONLY BE CALLED when the player is inside the platform in the next frame

	float leftPlayer = player.x - player.height / 2.0f;
	float rightPlayer = player.x + player.height / 2.0f;
	float topPlayer = player.y - player.height / 2.0f;
	float bottomPlayer = player.y + player.height / 2.0f;

	// This code takes in player's corner coordinates
	// as well as velocity to get next frame's coordinates
	// And compares it with the given corner of platform 

	// scenario determines which corner of the platfrom was given
	// 1 is top left
	// 2 is top right
	// 3 is bottom left
	// 4 is bottom right

	// First step: determine which corner was given basen on last player coordinates and platform coordinates
	if (scenario == 1) { // Player is left of platform, moving downwards
		// As player is moving towards its bottom right, BOTH hori velo and vert velo is +ve
		float distanceToPlatformX = platfromCornerX - rightPlayer; // Always +ve (if player is not directly under Platform)
		float distanceToPlatformY = platformCornerY - bottomPlayer; // Might be -ve
		float playerDistanceCoveredX = player.horizontalVelocity * CP_System_GetDt();
		float playerDistanceCoveredY = player.verticalVelocity * CP_System_GetDt();

		float ratioOfDistance = distanceToPlatformX / distanceToPlatformY;
		float ratioOfPlayerDistance = playerDistanceCoveredX / playerDistanceCoveredY;
		if (ratioOfDistance > ratioOfPlayerDistance) { // Player Y ratio higher than X. Player Face Plant
			return 1;
		} else { // Player X ratio higher than Y. Player landed on platform first
			return 0;
		}
	} else if (scenario == 2) { // Player is right of platform, moving downwards
		// As player is moving towards its bottom left, hori velo is -ve and vert velo is +ve
		float distanceToPlatformX = leftPlayer - platfromCornerX; // Always +ve (if player is not directly under Platform)
		float distanceToPlatformY = platformCornerY - bottomPlayer; // Always +ve
		float playerDistanceCoveredX = -player.horizontalVelocity * CP_System_GetDt(); // Set to Positive
		float playerDistanceCoveredY = player.verticalVelocity * CP_System_GetDt();

		float ratioOfDistance = distanceToPlatformX / distanceToPlatformY;
		float ratioOfPlayerDistance = playerDistanceCoveredX / playerDistanceCoveredY;
		if (ratioOfDistance > ratioOfPlayerDistance) { // Player Y ratio higher than X. Player Face Plant
			return 1;
		} else { // Player X ratio higher than Y-> Player landed on platform first->
			return 0;
		}
	} else if (scenario == 3) { // Player is left of platform, moving upwards
		// As player is moving towards its top right, hori velo is +ve and vert velo is -ve
		float distanceToPlatformX = platfromCornerX - rightPlayer; // Always +ve (if player is not directly under Platform)
		float distanceToPlatformY = topPlayer - platformCornerY; // Might be -ve
		float playerDistanceCoveredX = player.horizontalVelocity * CP_System_GetDt();
		float playerDistanceCoveredY = -player.verticalVelocity * CP_System_GetDt(); // Set to Postive for calculating distance

		float ratioOfDistance = distanceToPlatformX / distanceToPlatformY;
		float ratioOfPlayerDistance = playerDistanceCoveredX / playerDistanceCoveredY;
		if (ratioOfDistance > ratioOfPlayerDistance) { // Player Y ratio higher than X. Player Face Plant
			return 1;
		} else { // Player X ratio higher than Y. Player head hit platform first
			return 0;
		}
	} else { // (scenario == 4) Player is right of platform, moving upwards
		// As player is moving towards its top left, BOTH hori velo and vert velo is -ve
		float distanceToPlatformX = leftPlayer - platfromCornerX; // Always +ve (if player is not directly under Platform)
		float distanceToPlatformY = topPlayer - platformCornerY; // Might be -ve
		float playerDistanceCoveredX = -player.horizontalVelocity * CP_System_GetDt(); // Set to Postive for calculating distance
		float playerDistanceCoveredY = -player.verticalVelocity * CP_System_GetDt(); // Set to Postive for calculating distance

		float ratioOfDistance = distanceToPlatformX / distanceToPlatformY;
		float ratioOfPlayerDistance = playerDistanceCoveredX / playerDistanceCoveredY;
		if (ratioOfDistance > ratioOfPlayerDistance) { // Player Y ratio higher than X Player Face Plant
			return 1;
		} else { // Player X ratio higher than Y. Player head hit platform first
			return 0;
		}
	}
}
