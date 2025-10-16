//------------------------------------------------------------------------------------
// file: enemy.c
//
// authors:
// tituswenshuen.kwong (primary author)
//		Added all functions regarding flying enemies
// l.kangweimatthew 
//		Added all functions regarding walking enemies 
//
// brief:
// Contains the function definitions for spawning flying and walking enemies and their respective behaviour logic 
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

#include <math.h>
#include <stdio.h>
#include "initialize.h"
#include "cprocessing.h"
#include "utils.h"
#include "pickups.h"

#pragma region FlyingEnemies

void ContinuousSpawningFlying(struct Flying_Enemy enemies[], int enemyCount, float maxY, int healthModifier, int speedModifier) {
	int count = 0;
	for (int i = 0; i < MAX_FLYING_ENEMIES; ++i) {
		if (!enemies[i].isActive) {
			int rng = RandomNumber(0, 100);
			enemies[i].x = (rng / 100.0f) * CP_System_GetWindowWidth();
			enemies[i].y = CP_System_GetWindowHeight() + maxY; // Set Y-coordinate from the bottom, with spacing
			enemies[i].enemyHealth = DIFFICULTY_MIN_FLYING_ENEMIES_HEALTH + healthModifier;
			enemies[i].enemySpeed = DIFFICULTY_MIN_FLYING_ENEMIES_SPEED + speedModifier;
			enemies[i].diameter = CP_System_GetWindowWidth() * 0.03f;
			enemies[i].isActive = 1;
			count++;
		}

		if (enemyCount <= count) {
			break;
		}
	}
}

void UpdateFlyingEnemies(struct Player player, struct Flying_Enemy enemies[]) {
	for (int i = 0; i < MAX_FLYING_ENEMIES; i++) {
		// Update enemy movements based on the player's position
		float dx = player.x - enemies[i].x; // Calculate the X distance to the player
		float dy = player.y - enemies[i].y; // Calculate the Y distance to the player

		float distance_to_player = sqrtf(dx * dx + dy * dy); // Calculate the distance

		if (distance_to_player > 0) { // Avoid division by zero
			// Calculate the normalized direction vector towards the player
			float dir_x = dx / distance_to_player;
			float dir_y = dy / distance_to_player;

			// Update the enemy's position to move toward the player
			enemies[i].x += dir_x * enemies[i].enemySpeed;
			enemies[i].y += dir_y * enemies[i].enemySpeed;
		}

		if (enemies[i].enemyHealth < 1) {
			enemies[i].isActive = 0;
		}
	}

}

void BulletHitFlyingEnemy(struct Bullet bullets[], struct Flying_Enemy enemies[], struct Player* player) {
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].isActive) {
			for (int j = 0; j < MAX_FLYING_ENEMIES; j++) {
				if (enemies[j].isActive) {
					// Check for collision between the bullet and the enemy.
					if (AreCirclesIntersecting(bullets[i].x, bullets[i].y, bullets[i].diameterSize, enemies[j].x, enemies[j].y, enemies[j].diameter)) {
						bullets[i].isActive = 0; // Deactivate the bullet.
						enemies[j].enemyHealth -= player->damage;
						if (enemies[j].enemyHealth <= 0) {
							player->killCount += 1;
							enemies[j].isActive = 0; // Deactivate the enemy.
						}
						break;
					}
				}
			}
		}
	}
}

void FlyingEnemyTouchPlayer(struct Flying_Enemy enemies[], struct Player* player) {
	for (int i = 0; i < MAX_FLYING_ENEMIES; i++) {
		if (enemies[i].isActive) {
			// Check for collision between the bullet and the enemy.
			if (IsCircleAndRectIntersecting(enemies[i].x, enemies[i].y, enemies[i].diameter, player->x, player->y, player->width, player->height)) {
				player->currentHealth -= 1;
				CP_Sound_Play(player->damagedSFX);
				enemies[i].isActive = 0;
			}
		}
	}
}

void DespawnFlyingEnemy(struct Flying_Enemy enem[], float maxY) {
	for (int i = 0; i < MAX_FLYING_ENEMIES; ++i) {
		if (enem[i].isActive && enem[i].y < maxY - CP_System_GetWindowHeight() / 2.0f) {
			enem[i].isActive = 0;
		}
	}
}

#pragma endregion

#pragma region WalkingEnemies

void ContinuousSpawningWalking(struct Walking_Enemy enem[], int enemyCount, struct Platform platforms[], int healthModifier, int speedModifier) {
	int count = 0;
	for (int i = 0; i < MAX_WALKING_ENEMIES; ++i) { // if i is less than max enemies
		// Do RNG
		int rng = RandomNumber(0, 99);
		int random = RandomNumber(1, 2);
		int directionRNG = rng % 2; // 0 / 1
		float platformHeight = (CP_System_GetWindowHeight() / 100.0f) / 2;
		if (!enem[i].isActive) { // if its not active
			// Set Data
			enem[i].width = (int)(3 * CP_System_GetWindowWidth() / 100.0f);
			enem[i].height = (int)(enem[i].width * 1.5f);
			enem[i].speed = DIFFICULTY_MIN_WALKING_ENEMIES_SPEED + speedModifier;
			enem[i].detectRange = 200;
			enem[i].health = DIFFICULTY_MIN_WALKING_ENEMIES_HEALTH + healthModifier;
			enem[i].dir = directionRNG;
			// Set Position
			if (platforms[5].p2Coord != 1600) { // if platform two does not exist
				if (random == 1) {
					enem[i].boundL = platforms[5].p1Coord - platforms[5].p1Length / 2.0f;
					enem[i].boundR = platforms[5].p1Coord + platforms[5].p1Length / 2.0f;
					enem[i].x = enem[i].boundL + rng / 100.0f * (enem[i].boundR - enem[i].boundL);
					enem[i].y = platforms[5].y * 100 - platformHeight - enem[i].height / 2.0f;
				} else {
					enem[i].boundL = platforms[5].p2Coord - platforms[5].p2Length / 2.0f;
					enem[i].boundR = platforms[5].p2Coord + platforms[5].p2Length / 2.0f;
					enem[i].x = enem[i].boundL + rng / 100.0f * (enem[i].boundR - enem[i].boundL);
					enem[i].y = platforms[5].y * 100 - platformHeight - enem[i].height / 2.0f;
				}
			} else {
				enem[i].boundL = platforms[5].p1Coord - platforms[5].p1Length / 2.0f;
				enem[i].boundR = platforms[5].p1Coord + platforms[5].p1Length / 2.0f;
				enem[i].x = enem[i].boundL + rng / 100.0f * (enem[i].boundR - enem[i].boundL);
				enem[i].y = platforms[5].y * 100 - platformHeight - enem[i].height / 2.0f;
			}
			if (enem[i].x != 0 && enem[i].x != 1600) { // if there was no error with the position set
				enem[i].isActive = 1;
			}
			count++;
			if (count >= enemyCount) { // if spawn count reaches set count
				break;
			}
		}
	}
}

void WalkingEnemyLogic(struct Player player, struct Walking_Enemy enem[]) {
	for (int i = 0; i < MAX_WALKING_ENEMIES; ++i) {
		if (enem[i].isActive) {
			float dx = player.x - enem[i].x; // Calculate the X distance to the player
			float dy = player.y - enem[i].y; // Calculate the Y distance to the player
			float distance_to_player = sqrtf(dx * dx + dy * dy); // Calculate the distance
			if (distance_to_player < enem[i].detectRange) {
				if (enem[i].x - enem[i].width / 2.0f > enem[i].boundL && enem[i].x + enem[i].width / 2.0f < enem[i].boundR) {
					float dir_x = dx / distance_to_player;
					enem[i].x += dir_x * enem[i].speed;
					if (dx > 0) {
						enem[i].dir = 1;
					} else {
						enem[i].dir = 0;
					}
				}
			} else {
				if (!enem[i].dir) { // 0 is left
					enem[i].x -= enem[i].speed * CP_System_GetDt() * 20;
				} else if (enem[i].dir) { // 1 is right
					enem[i].x += enem[i].speed * CP_System_GetDt() * 20;
				}
				if (enem[i].x - enem[i].width / 2.0f - 10 < enem[i].boundL) {
					enem[i].dir = 1;
				} else if (enem[i].x + enem[i].width / 2.0f + 10 > enem[i].boundR) {
					enem[i].dir = 0;
				}
			}
		}
		if (enem[i].health <= 0) {
			enem[i].isActive = 0;
		}
	}
}

void BulletHitWalkingEnemy(struct Bullet bullets[], struct Walking_Enemy enemies[], struct Player* player, struct Pickups pickup[]) {
	for (int i = 0; i < MAX_BULLETS; ++i) {
		if (bullets[i].isActive) {
			for (int j = 0; j < MAX_WALKING_ENEMIES; ++j) {
				if (enemies[j].isActive) {
					// Check for collision between the bullet and the enemy.
					if (IsCircleAndRectIntersecting(bullets[i].x, bullets[i].y, bullets[i].diameterSize, enemies[j].x, enemies[j].y, (float)enemies[j].width, (float)enemies[j].height)) {
						bullets[i].isActive = 0; // Deactivate the bullet.
						enemies[j].health -= player->damage;
						if (enemies[j].health <= 0) {
							enemies[j].isActive = 0;
							player->killCount += 1;
							SpawnHealthPickup(pickup, enemies[j].x, enemies[j].y, PICKUP_DROP_CHANCE);
						}
						break;
					}
				}
			}
		}
	}
}

void WalkingEnemyTouchPlayer(struct Walking_Enemy enemies[], struct Player* player) {
	for (int i = 0; i < MAX_WALKING_ENEMIES; i++) {
		if (enemies[i].isActive) {
			// Check for collision between the Player and the enemy.
			if (AreRectanglesIntersecting(enemies[i].x, enemies[i].y, (float)enemies[i].width, (float)enemies[i].height, player->x, player->y, player->width, player->height)) {
				player->currentHealth -= 1;
				CP_Sound_Play(player->damagedSFX);
				enemies[i].isActive = 0;
			}
		}
	}
}

void DespawnWalkingEnemy(struct Walking_Enemy enem[], float maxY) {
	for (int i = 0; i < MAX_WALKING_ENEMIES; ++i) {
		if (enem[i].isActive && enem[i].y < maxY - CP_System_GetWindowHeight() / 2.0f) {
			enem[i].isActive = 0;
		}
	}
}

#pragma endregion
