//------------------------------------------------------------------------------------
// file: initialize.c
//
// authors:
// l.zheneudamon (primary author)
//		Added Initialization for
//		Player, Game State Upgrade, Game State Defeat, Pickups, Lazer Hazards
// tituswenshuen.kwong
//		Added Initialising function for
//		Flying Enemy, Bullet
// l.kangweimatthew
//		Added Initialising function for
//		Walking Enemy
//
// brief:
// Contains the Initialize functions of the game.
// Sets these values to defaults when game is started / restarted.
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

void InitializePlayer(struct Player* p) {
	p->imageAddress = NULL;
	p->width = 2 * CP_System_GetWindowWidth() / 100.0f; // 0.02 of Window_Width
	p->height = p->width * 3; // 0.06 of Window_Width
	p->x = CP_System_GetWindowWidth() / 2.0f;
	p->y = CP_System_GetWindowHeight() / 2.0f - 100;
	p->horizontalVelocity = 0;
	p->verticalVelocity = 0;
	p->movementSpeed = 150;
	p->jumpSpeed = 500;
	p->maxHealth = 3;
	p->currentHealth = 3;
	p->damage = 50; // Starting Bullet Damage
	p->projectileSize = 30; // Starting Bullet Size
	p->projectileSpeed = 500; // Starting Bullet Speed
	p->fireRate = 2; // Starting with 2 Bullets/Second Speed
	p->killCount = 0;
	p->totalUpgrades = 0;
	for (int i = 0; i < MAX_UPGRADES; ++i) {
		p->upgrades[i] = 0;
	}
}

void InitializeGameStateUpgrade(struct Game_State_Upgrade *p, float playerX) {
	p->chosenUpgrade = 0;
	p->isUpgradeChosen = 0;
	p->secondsSinceState = 0;
	p->secondsSinceUpgradeChosen = 0;
	p->distanceToCenter = playerX - CP_System_GetWindowWidth() / 2.0f;
	for (int i = 0; i < 3; ++i) {
		p->upgradeChoice[i] = 0;
	}
}

void InitializeGameStateDefeat(struct Game_State_Defeat *p) {
	p->secondsSinceState = 0;
}

void InitializeBullets(struct Bullet bullets[]) {
	for (int i = 0; i < MAX_BULLETS; ++i) {
		bullets[i].isActive = 0; // Sets all Bullets to Inactive
	}
}

void InitializeWalkingEnemies(struct Walking_Enemy enemies[]) {
	for (int i = 0; i < MAX_WALKING_ENEMIES; ++i) {
		enemies[i].x = 0; 
		enemies[i].y = 0;
		enemies[i].isActive = 0;
		enemies[i].boundL = 0;
		enemies[i].boundR = 0;
		enemies[i].width = 0;
		enemies[i].height = 0;
		enemies[i].health = 0;
		enemies[i].dir = 0;
		enemies[i].speed = 0;
		enemies[i].detectRange = 0;
	}
}

void InitializeFlyingEnemies(struct Flying_Enemy enemies[]) {
	for (int i = 0; i < MAX_FLYING_ENEMIES; ++i) {
		enemies[i].x = 0;
		enemies[i].y = 0;
		enemies[i].isActive = 0;
		enemies[i].diameter = 0;
		enemies[i].enemySpeed = 0;
		enemies[i].enemyHealth = 0;
	}
}

void InitializePickups(struct Pickups pickup[]) {
	for (int i = 0; i < MAX_HEALTH_PICKUPS; ++i) {
		pickup[i].x = 0;
		pickup[i].y = 0;
		pickup[i].isActive = 0;
		pickup[i].imageSize = 0;
		pickup[i].imageAddress = NULL;
	}
}

void InitializeLazerHazard(struct Lazer_Hazard lazerHazards[]) {
	for (int i = 0; i < MAX_LAZER_HAZARDS; ++i) {
		lazerHazards[i].y = 0;
		lazerHazards[i].height = 0;
		lazerHazards[i].isActive = 0;
		lazerHazards[i].timeToFire = 0;
		lazerHazards[i].timeCreated = 0;
	}
}
