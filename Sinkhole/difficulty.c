//------------------------------------------------------------------------------------
// file: difficulty.c
//
// authors:
// l.kangweimatthew (primary author)
//		Created the two functions that handles the scaling of difficulty and scaling of the game
// l.zheneudamon 
//		Assisted in creating the function logic and the balancing of the difficulty 
//
// brief:
// Contains the function defintions that control the difficulty scaling and logic of the game
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

#include "cprocessing.h"
#include "utils.h"
#include "enemy.h"
#include "initialize.h"
#include "hazards.h"

void DifficultyPlatformModifier(int playerTotalUpgrades, struct Walking_Enemy walkingEnemies[], struct Platform platforms[]) {
	int spawnChance = DIFFICULTY_MIN_SPAWN_PERCENTAGE_MODIFIER; // min chance to spawn
	int spawnNumber = DIFFICULTY_MIN_WALKING_ENEMIES; // max chance to spawn
	spawnChance = spawnChance > DIFFICULTY_MAX_SPAWN_PERCENTAGE_MODIFIER ? DIFFICULTY_MAX_SPAWN_PERCENTAGE_MODIFIER : spawnChance + DIFFICULTY_SPAWN_PERCENTAGE_INCREMENT_MODIFIER * playerTotalUpgrades; // increase the spawn chance by 5% every player upgrade but cap at max modifier
	spawnNumber = spawnNumber > DIFFICULTY_MAX_WALKING_ENEMIES ? DIFFICULTY_MAX_WALKING_ENEMIES : spawnNumber + playerTotalUpgrades / 2; // increase the amount of enemy to spawn every 2 player upgrades but cap at the max modifier
	int rngSpawnChance = RandomNumber(0, 99); // random chance out of 100
	int rngSpawnNumber = RandomNumber(1, spawnNumber); // random chance of 1 to spawnNumber as the number to spawn ( 1 to 3 )

	int incrementMultiplierWalkingEnemies = playerTotalUpgrades / 2; // Once every 2 upgrades
	if (rngSpawnChance < spawnChance) { 
		ContinuousSpawningWalking(walkingEnemies, rngSpawnNumber, platforms, DIFFICULTY_HEALTH_INCREMENT_WALKING_ENEMIES_MODIFIER * incrementMultiplierWalkingEnemies, DIFFICULTY_SPEED_INCREMENT_WALKING_ENEMIES_MODIFIER * incrementMultiplierWalkingEnemies);
	}
}

void DifficultyModifier(int playerTotalUpgrades, struct Flying_Enemy flyingEnemies[], struct Lazer_Hazard lazerHazard[], float maxY, float gameTimer, float *lastSpawnedTime, float *lazerSpawnedTimer, int *nextLazerTimer) {
	float const lazerTimeToFireIncrement = 0.25;
	float lazerTimeToFire = DIFFICULTY_MIN_HAZARD_TIME_TO_FIRE_MODIFIER;
	float lazerHeight = DIFFICULTY_MIN_HAZARD_HEIGHT_MODIFIER;
	if (playerTotalUpgrades >= 2 && gameTimer > (*lastSpawnedTime + 4)) { // if player reaches 2 powerups and 4 seconds since last spawn
		int incrementMultiplierFlyingEnemies = (playerTotalUpgrades - 2) / 2; // Once every 2 Upgrades past upgrade number 4
		*lastSpawnedTime = gameTimer;
		ContinuousSpawningFlying(flyingEnemies, 1, maxY, DIFFICULTY_MIN_FLYING_ENEMIES_HEALTH + DIFFICULTY_HEALTH_INCREMENT_FLYING_ENEMIES_MODIFIER * incrementMultiplierFlyingEnemies, DIFFICULTY_MIN_FLYING_ENEMIES_SPEED + DIFFICULTY_SPEED_INCREMENT_FLYING_ENEMIES_MODIFIER * incrementMultiplierFlyingEnemies);
	}

	if (playerTotalUpgrades >= 4 && gameTimer > (*lazerSpawnedTimer + *nextLazerTimer)) {
		*lazerSpawnedTimer = gameTimer;
		*nextLazerTimer = RandomNumber(DIFFICULTY_MIN_HAZARD_SPAWN_TIME, DIFFICULTY_MAX_HAZARD_SPAWN_TIME);
		lazerTimeToFire = lazerTimeToFire < DIFFICULTY_MAX_HAZARD_TIME_TO_FIRE_MODIFIER ? DIFFICULTY_MAX_HAZARD_TIME_TO_FIRE_MODIFIER : lazerTimeToFire - lazerTimeToFireIncrement * (float)((playerTotalUpgrades - 4) / 2);
		float lazerY = (float)RandomNumber((int)maxY - (CP_System_GetWindowHeight() / 4), (int)maxY + (CP_System_GetWindowHeight() / 4)); // Can spawn from top 25% to 75% of Window Height
		lazerHeight = lazerHeight > DIFFICULTY_MAX_HAZARD_HEIGHT_MODIFIER ? DIFFICULTY_MAX_HAZARD_HEIGHT_MODIFIER : lazerHeight + DIFFICULTY_HAZARD_HEIGHT_INCREMENT_MODIFIER * (playerTotalUpgrades - 4);
		SpawnLazerHazard(lazerHazard, lazerTimeToFire, lazerY, lazerHeight, gameTimer);
	}
}
