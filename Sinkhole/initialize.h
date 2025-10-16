#pragma once
//------------------------------------------------------------------------------------
// file: initialize.h
//
// authors:
// l.zheneudamon (primary author)
//		Added Structs for 
//		Player, Lazer, Upgrades, Pickups, Game State Upgrade, Game State Defeat
// timo.duethorn
//		Added Platform Struct
// tituswenshuen.kwong
//		Added Structs for 
//		Flying Enenmy, Bullet
//		Added initialising function for
//		Flying Enemy, Bullet
// l.kangweimatthew
//		Added Structs for 
//		Walking Enenmy
//		Added initialising function for
//		Walking Enemy
// 
// brief:
// Contains function declaration of functions in initialize.c.
// Also contains structs used throughout the project files.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

#include <cprocessing.h>

enum {
	// Game States
	GAME_STATE_PLAYING = 1,
	GAME_STATE_UPGRADE = 2,
	GAME_STATE_DEFEAT = 3,
	GAME_STATE_PAUSE = 4,

	GRAVITY = 600,
	TERMINAL_VELOCITY = 100,
	FIRST_UPGRADE_Y_VALUE = 2000, // First Upgrade is at Y 2000
	NEXT_UPGRADE_Y_VALUE = 5000, // Next Upgrade is Y 2000 + Y 5000 * numberOfUpgrades
	FALLING_Y_VALUE = 1000, // Fall for this distance after getting an upgrade
	MAX_ASSET_FILEPATH = 256,
	MAX_UPGRADES = 11,
	MAX_UPGRADE_NAME = 128,
	MAX_UPGRADE_TEXT = 256,
	MAX_BULLETS = 20,
	MAX_PLATFORMS = 6,
	MAX_SCORE_DIGITS = 20, // Highscore wont be more than 10^19
	MAX_KILLS_DIGITS = 7, // Kills wont be more than 10^6
	MAX_HEALTH_PICKUPS = 5, // No more than 5 health pickups in the screen at anypoint of time
	MAX_LAZER_HAZARDS = 10, // No more than 10 lazer hazards at anypoint of time
	MAX_WALKING_ENEMIES = 50, // No more than 50 walking enemies in the screen at anypoint of time
	MAX_FLYING_ENEMIES = 10, // No more than 10 flying enemies at anypoint of time

	// Number of frame for animations
	MAX_FRAME_DIGITS = 2, // No more than 99 frames per asset please
	MAX_FRAME_PLAYER_CRYSTAL = 2,
	MAX_FRAME_PLAYER_DEFEAT = 2,
	MAX_FRAME_PLAYER_IDLE = 2,
	MAX_FRAME_PLAYER_RIGHT = 2,
	MAX_FRAME_PLAYER_LEFT = 2,
	MAX_FRAME_PLAYER_JUMP = 1,
	MAX_FRAME_PLAYER_JUMP_RIGHT = 1,
	MAX_FRAME_PLAYER_JUMP_LEFT = 1,
	MAX_FRAME_PLAYER_FALL = 1,
	MAX_FRAME_PLAYER_FALL_RIGHT = 1,
	MAX_FRAME_PLAYER_FALL_LEFT = 1,
	MAX_FRAME_PICKUP_HEALTH = 2,
	MAX_FRAME_ENEMY_FLYING_LEFT = 1,
	MAX_FRAME_ENEMY_FLYING_RIGHT = 1,
	MAX_FRAME_ENEMY_WALKING_LEFT = 1,
	MAX_FRAME_ENEMY_WALKING_RIGHT = 1,

	// Upgrade Increments
	UPGRADE_DAMAGE_INCREMENT = 10,
	UPGRADE_SPEED_INCREMENT = 50,
	UPGRADE_FIRERATE_INCREMENT = 2,
	UPGRADE_PROJECTILESIZE_INCREMENT = 30,
	UPGRADE_PROJECTILESPEED_INCREMENT = 200,

	// Difficulty Increments
	DIFFICULTY_MAX_FLYING_ENEMIES = 5, // In Map
	DIFFICULTY_HEALTH_INCREMENT_FLYING_ENEMIES_MODIFIER = 5,
	DIFFICULTY_MIN_FLYING_ENEMIES_HEALTH = 10,
	DIFFICULTY_SPEED_INCREMENT_FLYING_ENEMIES_MODIFIER = 2,
	DIFFICULTY_MIN_FLYING_ENEMIES_SPEED = 2,

	DIFFICULTY_MIN_WALKING_ENEMIES = 1, // Per Platform
	DIFFICULTY_MAX_WALKING_ENEMIES = 3, // Per Platform
	DIFFICULTY_HEALTH_INCREMENT_WALKING_ENEMIES_MODIFIER = 20,
	DIFFICULTY_MIN_WALKING_ENEMIES_HEALTH = 20,
	DIFFICULTY_SPEED_INCREMENT_WALKING_ENEMIES_MODIFIER = 2,
	DIFFICULTY_MIN_WALKING_ENEMIES_SPEED = 3,

	DIFFICULTY_MIN_SPAWN_PERCENTAGE_MODIFIER = 25,
	DIFFICULTY_MAX_SPAWN_PERCENTAGE_MODIFIER = 75,
	DIFFICULTY_SPAWN_PERCENTAGE_INCREMENT_MODIFIER = 5,

	DIFFICULTY_MIN_HAZARD_TIME_TO_FIRE_MODIFIER = 3, // Time to fire increment not here as value is 0.25
	DIFFICULTY_MAX_HAZARD_TIME_TO_FIRE_MODIFIER = 1,
	DIFFICULTY_MIN_HAZARD_HEIGHT_MODIFIER = 50,
	DIFFICULTY_HAZARD_HEIGHT_INCREMENT_MODIFIER = 10,
	DIFFICULTY_MAX_HAZARD_HEIGHT_MODIFIER = 100,
	DIFFICULTY_MIN_HAZARD_SPAWN_TIME = 5,
	DIFFICULTY_MAX_HAZARD_SPAWN_TIME = 8,

	PICKUP_DROP_CHANCE = 5,
	PICKUP_SIZE = 50,

};

struct Platform {
	float p1Coord, p2Coord;
	int y, start, p1Length, p2Length, g1Length, g2Length;
};

struct Player {
	float x, y, verticalVelocity, horizontalVelocity;
	float width, height;
	int maxHealth, currentHealth; // Health Variables (UI)
	int movementSpeed, jumpSpeed; // Movement Variables

	CP_Image *imageAddress;

	int damage; // Bullet Damage
	int isShooting; // Boolean
	int upgrades[MAX_UPGRADES];
	int totalUpgrades; // Basically adds up all of upgrades[]

	// Shooting Variables 
	int fireRate; // Bullets/Second
	int projectileSize; // Diameter of Bullet
	int projectileSpeed; // Speed of Bullets 

	int killCount;

	// Sound Effects
	CP_Sound jumpSFX;
	CP_Sound damagedSFX;
	CP_Sound pickupSFX;
};

struct Bullet {
	float x, y; // Position of the bullet
	float velocity; // Speed of the bullet
	float diameterSize; // Diameter of the bullet
	float distanceTraveled; // The distance the bullet has traveled (for limiting range)
	float directionX, directionY; // Direction vector of the bullet
	int isActive; // Indicates if the bullet is active
};

struct Flying_Enemy {
	float x, y, diameter;
	int enemyHealth, isActive, enemySpeed;
	CP_Image *imageAddress;
};

struct Walking_Enemy {
	float x, y, boundR, boundL;
	int width, height, health, detectRange, speed, isActive, dir;
	CP_Image* imageAddress;
};

struct Lazer_Hazard {
	float y, height;
	float timeToFire, timeCreated;
	int isActive;
};

struct Upgrades {
	char upgradeName[MAX_UPGRADE_NAME];
	char upgradeText[MAX_UPGRADE_TEXT];
};

struct Pickups {
	float x, y;
	float imageSize;
	int isActive;
	CP_Image *imageAddress;
};

struct Game_State_Upgrade {
	int isUpgradeChosen; // 0 for upgrade not chosen, 1 for upgrade chosen
	int upgradeChoice[3]; // 3 different upgrades to choose from
	int chosenUpgrade; // 1 for left most, 2 for middle, 3 for right most
	float distanceToCenter;
	float secondsSinceState;
	float secondsSinceUpgradeChosen;
};

struct Game_State_Defeat {
	float secondsSinceState;
};

/// <summary>
/// Initializes the Player
/// </summary>
/// <param name="p">Address of the Player</param>
void InitializePlayer(struct Player* p);

/// <summary>
/// Initializes the Game State for Upgrade
/// </summary>
/// <param name="p">Address of Created Game State</param>
/// <param name="playerX">Player's X Value</param>
void InitializeGameStateUpgrade(struct Game_State_Upgrade* p, float playerX);

/// <summary>
/// Initializes the Game State for Defeat
/// </summary>
/// <param name="p">Address of Created Game State</param>
void InitializeGameStateDefeat(struct Game_State_Defeat* p);

/// <summary>
/// Initializes the Bullet Struct Array
/// </summary>
/// <param name="bullets">Array of Bullets</param>
void InitializeBullets(struct Bullet bullets[]);

/// <summary>
/// Initializes Flying Enemies
/// </summary>
/// <param name="enemies">Array of Flying Enemies</param>
void InitializeFlyingEnemies(struct Flying_Enemy enemies[]);

/// <summary>
/// Initializes the Walking_Enemy Struct Array
/// </summary>
/// <param name="enem">Array of Walking Enemies</param>
void InitializeWalkingEnemies(struct Walking_Enemy enem[]);

/// <summary>
/// Intitializes Pickups
/// </summary>
/// <param name="pickup">Array of Pickups</param>
void InitializePickups(struct Pickups pickup[]);

/// <summary>
/// Initializes Lazer Hazards
/// </summary>
/// <param name="lazerHazards">Array of Lazer Hazards</param>
void InitializeLazerHazard(struct Lazer_Hazard lazerHazards[]);
