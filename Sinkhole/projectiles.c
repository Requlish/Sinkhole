//------------------------------------------------------------------------------------
// file: projectiles.c
//
// authors:
// tituswenshuen.kwong
//		Added bullet shooting functions
//
// brief:
// Contains the function definitions that deal with the shooting logic of bullets
// FireBullet - player bullet firing
// UpdateBullet - bullet updating after hitting enemy, and side walls
// BulletHitPlatform - bullet updating after hitting platforms
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

#include <math.h>
#include "cprocessing.h"
#include "initialize.h"
#include "utils.h"

// Fire Bullets
void FireBullet(struct Bullet bullets[], struct Player player, float mouseX, float mouseY) {
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (!bullets[i].isActive) { // Find an inactive bullet to use
			// Set the bullet's starting position to the player's (crystal) position
			bullets[i].x = player.x;
			bullets[i].y = player.y - (player.height + player.width) / 2.0f;

			// Calculate the direction vector from player (crystal) to mouse
			float directionX = mouseX - bullets[i].x;
			float directionY = mouseY - bullets[i].y;

			// Calculate the direction's magnitude and normalize it (make it a unit vector)
			float directionMagnitude = sqrtf(directionX * directionX + directionY * directionY);
			float normalizedDirectionX = directionX / directionMagnitude;
			float normalizedDirectionY = directionY / directionMagnitude;

			bullets[i].diameterSize = (float)player.projectileSize; // Set the bullet's size
			bullets[i].velocity = (float)player.projectileSpeed; // Set the bullet's speed
			bullets[i].directionX = normalizedDirectionX; // Store the X component of the normalized direction
			bullets[i].directionY = normalizedDirectionY; // Store the Y component of the normalized direction
			bullets[i].isActive = 1;
			bullets[i].distanceTraveled = 0.0; // Reset the distance traveled by the bullet

			break;
		}
	}
}

// Update Bullets
void UpdateBullets(struct Bullet bullets[], float maxY, float wallWidth) {
	float deltaTime = CP_System_GetDt();
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].isActive) { // Check if the bullet is active
			// Update the bullet's position using both X and Y components of the normalized direction vector
			bullets[i].x += bullets[i].velocity * deltaTime * bullets[i].directionX;
			bullets[i].y += bullets[i].velocity * deltaTime * bullets[i].directionY;
			bullets[i].distanceTraveled += bullets[i].velocity * deltaTime;

			// Check for collision with left wall, right wall, screen top, and screen bottom
			// If hit boundaries, set to not active
			if (bullets[i].x < wallWidth) {
				bullets[i].isActive = 0;
			} else if (bullets[i].x > (CP_System_GetWindowWidth() - wallWidth)) {
				bullets[i].isActive = 0;
			} else if (bullets[i].y < maxY - CP_System_GetWindowHeight() / 2.0) {
				bullets[i].isActive = 0;
			} else if (bullets[i].y > maxY + CP_System_GetWindowHeight() / 2.0) {
				bullets[i].isActive = 0;
			}
		}
	}
}

// Update Bullet Conditions
void BulletHitPlatform(struct Bullet bullets[], struct Platform platforms[], float platformHeight) {
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].isActive) {
			for (int j = 0; j < MAX_PLATFORMS; j++) {
				float platform1x = platforms[j].p1Coord, platform1y = ((platforms[j].y) * 100.0f), platform1Width = (float)platforms[j].p1Length, platform1Height = platformHeight;
				float platform2x = platforms[j].p2Coord, platform2y = ((platforms[j].y) * 100.0f), platform2Width = (float)platforms[j].p2Length, platform2Height = platformHeight;
				if (IsCircleAndRectIntersecting(bullets[i].x, bullets[i].y, bullets[i].diameterSize, platform1x, platform1y, platform1Width, platform1Height)) { // Platform 1
					bullets[i].isActive = 0;
					break;
				} else if (IsCircleAndRectIntersecting(bullets[i].x, bullets[i].y, bullets[i].diameterSize, platform2x, platform2y, platform2Width, platform2Height)) { // Platform 2
					bullets[i].isActive = 0;
					break;
				}
			}
		}
	}
}
