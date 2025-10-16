//------------------------------------------------------------------------------------
// file: game.c
//
// authors:
// l.zheneudamon (primary author)
//		Created Game States
//		Added Collision
//		Added Game State Upgrade
//		Added User Input
//		Added Animations
//		Added Sound Effects
// timo.duethorn
//		Added Platform Integration (Generation and Rendering)
//		Added Drawing of Looping Background and Sidewalls
//		Added Information Side Bar (depth, kills, and multiplier)
//		Added Music
// tituswenshuen.kwong
//		Added Tooltips
//		Added Flying Enemy Spawning
//		Added Player Shooting
// l.kangweimatthew
//		Added Camera Movement
//		Added Gameplay Boundary Collision
//		Added Walking Enemy Spawning
//
// brief:
// Contains the main logic of the game. Initializes all required
// values for the game and handles the changing of game states.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <cprocessing.h>
#include "initialize.h"
#include "utils.h"
#include "defeat.h"
#include "pickups.h"
#include "graphics.h"
#include "mainmenu.h"
#include "pause.h"
#include "physics.h"
#include "platforms.h"
#include "restart.h"
#include "upgrades.h"
#include "projectiles.h"
#include "enemy.h"
#include "hazards.h"
#include "difficulty.h"

// Variable Declarations
struct Game_State_Upgrade gameStateUpgrade;
struct Game_State_Defeat gameStateDefeat;
struct Upgrades upgrades[MAX_UPGRADES] = { 0 };
struct Pickups pickups[MAX_HEALTH_PICKUPS] = { 0 };
struct Lazer_Hazard lazerHazards[MAX_LAZER_HAZARDS] = { 0 };
struct Platform platforms[6] = { 0 };
struct Bullet bullets[MAX_BULLETS];
struct Player player;
struct Flying_Enemy flyingEnemies[MAX_FLYING_ENEMIES] = { 0 };
struct Walking_Enemy walkingEnemies[MAX_WALKING_ENEMIES] = { 0 };

FILE* upgradesCSV;

errno_t err;

CP_Font upgradeFont, scoreFont;
CP_Color white, darkGrey, black, brown, red;

CP_Image bulletImage, playerCrystal[MAX_FRAME_PLAYER_CRYSTAL];
CP_Image mainBackgroundImage;
CP_Image brickSideWalls;
CP_Image damagedScreen;
CP_Image healthEmptyUI, healthFullUI;
CP_Image upgradeIcon[MAX_UPGRADES];
CP_Image pickupsHealth[MAX_FRAME_PICKUP_HEALTH];
CP_Image playerDefeat[MAX_FRAME_PLAYER_DEFEAT];
CP_Image playerIdle[MAX_FRAME_PLAYER_IDLE];
CP_Image playerJump[MAX_FRAME_PLAYER_JUMP];
CP_Image playerFall[MAX_FRAME_PLAYER_FALL];
CP_Image playerRight[MAX_FRAME_PLAYER_RIGHT];
CP_Image playerJumpRight[MAX_FRAME_PLAYER_JUMP_RIGHT];
CP_Image playerFallRight[MAX_FRAME_PLAYER_FALL_RIGHT];
CP_Image playerLeft[MAX_FRAME_PLAYER_LEFT];
CP_Image playerJumpLeft[MAX_FRAME_PLAYER_JUMP_LEFT];
CP_Image playerFallLeft[MAX_FRAME_PLAYER_FALL_LEFT];
CP_Image playerUpgradeFalling;
CP_Image tutorial_playerControls;
CP_Image tutorial_playerUpgrades;
CP_Image tutorial_playerShooting;
CP_Image tutorial_flyingEnemiesWarning;
CP_Image tutorial_laserWarning;

CP_Image ememyWalkingLeft[MAX_FRAME_ENEMY_WALKING_LEFT];
CP_Image ememyWalkingRight[MAX_FRAME_ENEMY_WALKING_RIGHT];
CP_Image ememyFlyingLeft[MAX_FRAME_ENEMY_FLYING_LEFT];
CP_Image ememyFlyingRight[MAX_FRAME_ENEMY_FLYING_RIGHT];

CP_Sound gameTrack = NULL;

int gameState = GAME_STATE_PLAYING;

float maxY = 0; // Maximum Y the player has reached. (Camera follows this value)
float wallWidth;
float platformHeight;
float scoreMultipler;
float gameTimer = 0;
float lastShotTimer = 0;
float lastSpawnTimer = 0;
float lazerSpawnedTimer = 0;
int nextLazerTimer = 0;
int isTouching = 0;
int doubleJumpUsed = 0;
int isLeftOrRightOfPlatform;
int fallingThroughPlatform = 0;

int lastPlayerHeightThreshold;
int distanceBetweenPlatform;
int formattedPlayerY;
int playMusicOnce;

float backgroundHeight1, backgroundHeight2;
float leftWallCoord, rightWallCoord;

// Input Flags
int wKeyPressed = 0;
int wKeyTriggered = 0;
int aKeyPressed = 0;
int sKeyPressed = 0;
int dKeyPressed = 0;
int escKeyPressed = 0;
int leftMouseClick = 0;
int leftMouseHold = 0;

void Game_Init(void) {

	#pragma region readFiles

	int readChar, numberOfUpgrades = 0, afterUpgradeName = 0;
	err = fopen_s(&upgradesCSV,"Assets/Data/upgrades.csv", "r");
	if (err == 0) {
		for (int i = 0; (readChar = fgetc(upgradesCSV)) != EOF; ++i) {
			if (readChar == ',') {
				upgrades[numberOfUpgrades].upgradeName[i] = '\0'; // Turn Character Array into String
				afterUpgradeName = 1;
				i = -1;
			} else if (readChar == '\n') {
				upgrades[numberOfUpgrades].upgradeText[i] = '\0'; // Turn Character Array into String
				afterUpgradeName = 0;
				numberOfUpgrades++;
				i = -1;
			} else {
				if (afterUpgradeName) {
					upgrades[numberOfUpgrades].upgradeText[i] = readChar;
				} else {
					upgrades[numberOfUpgrades].upgradeName[i] = readChar;
				}
			}
		}
		fclose(upgradesCSV);
	} else {
		// Nothing
	}

	#pragma endregion

	#pragma region loadSounds

	gameTrack = CP_Sound_LoadMusic("Assets/Music/sermon_from_the_pit.mp3");

	player.jumpSFX = CP_Sound_Load("Assets/Music/SFX/Player/player_jump.wav");
	player.damagedSFX = CP_Sound_Load("Assets/Music/SFX/Player/player_damaged.wav");
	player.pickupSFX = CP_Sound_Load("Assets/Music/SFX/Player/pickup_health.wav");

	#pragma endregion

	#pragma region loadImages

	bulletImage = CP_Image_Load("Assets/Images/Projectiles/bullet.png");
	healthEmptyUI = CP_Image_Load("Assets/Images/UI/empty_heart.png");
	healthFullUI = CP_Image_Load("Assets/Images/UI/full_heart.png");
	playerUpgradeFalling = CP_Image_Load("Assets/Images/Player/player_upgrade_falling.png");
	damagedScreen = CP_Image_Load("Assets/Images/Background/damaged.png");
	mainBackgroundImage = CP_Image_Load("Assets/Images/Background/background.png");
	brickSideWalls = CP_Image_Load("Assets/Images/Background/brickwalls.png");
	tutorial_playerControls = CP_Image_Load("Assets/Images/Tutorial/movement.png");
	tutorial_playerUpgrades = CP_Image_Load("Assets/Images/Tutorial/upgrades.png");
	tutorial_playerShooting = CP_Image_Load("Assets/Images/Tutorial/shooting.png");
	tutorial_flyingEnemiesWarning = CP_Image_Load("Assets/Images/Tutorial/flying_enemies_warning.png");
	tutorial_laserWarning = CP_Image_Load("Assets/Images/Tutorial/laser_warning.png");

	for (int i = 0; i < MAX_UPGRADES; ++i) {
		char filePath[MAX_ASSET_FILEPATH] = "Assets/Images/UI/upgrade_";
		char upgradeNameWithoutSpace[MAX_UPGRADE_NAME] = "";
		for (int j = 0; j < MAX_UPGRADE_NAME && upgrades[i].upgradeName[j] != '\0';j++) {
			if (upgrades[i].upgradeName[j] == ' ') {
				upgradeNameWithoutSpace[j] = '_';
			} else {
				upgradeNameWithoutSpace[j] = (char)tolower(upgrades[i].upgradeName[j]);
			}
		}
		upgradeNameWithoutSpace[MAX_UPGRADE_NAME - 1] = '\0'; // Turn Character Array into String
		strcat_s(filePath, MAX_ASSET_FILEPATH, upgradeNameWithoutSpace);
		strcat_s(filePath, MAX_ASSET_FILEPATH, ".png");
		upgradeIcon[i] = CP_Image_Load(filePath);
	}

	AnimationImageLoader(playerCrystal, "Player/crystal_", MAX_FRAME_PLAYER_CRYSTAL);
	AnimationImageLoader(playerDefeat, "Player/player_defeat_", MAX_FRAME_PLAYER_DEFEAT);
	AnimationImageLoader(playerIdle, "Player/player_idle_", MAX_FRAME_PLAYER_IDLE);
	AnimationImageLoader(playerJump, "Player/player_jump_", MAX_FRAME_PLAYER_JUMP);
	AnimationImageLoader(playerFall, "Player/player_fall_", MAX_FRAME_PLAYER_FALL);
	AnimationImageLoader(playerRight, "Player/player_right_", MAX_FRAME_PLAYER_RIGHT);
	AnimationImageLoader(playerJumpRight, "Player/player_jump_right_", MAX_FRAME_PLAYER_JUMP_RIGHT);
	AnimationImageLoader(playerFallRight, "Player/player_fall_right_", MAX_FRAME_PLAYER_FALL_RIGHT);
	AnimationImageLoader(playerLeft, "Player/player_left_", MAX_FRAME_PLAYER_LEFT);
	AnimationImageLoader(playerJumpLeft, "Player/player_jump_left_", MAX_FRAME_PLAYER_JUMP_LEFT);
	AnimationImageLoader(playerFallLeft, "Player/player_fall_left_", MAX_FRAME_PLAYER_FALL_LEFT);
	AnimationImageLoader(pickupsHealth, "Pickups/health_", MAX_FRAME_PICKUP_HEALTH);

	AnimationImageLoader(ememyWalkingLeft, "Enemy/enemy_walking_left_", MAX_FRAME_ENEMY_WALKING_LEFT);
	AnimationImageLoader(ememyWalkingRight, "Enemy/enemy_walking_right_", MAX_FRAME_ENEMY_WALKING_RIGHT);
	AnimationImageLoader(ememyFlyingLeft, "Enemy/enemy_flying_left_", MAX_FRAME_ENEMY_FLYING_LEFT);
	AnimationImageLoader(ememyFlyingRight, "Enemy/enemy_flying_right_", MAX_FRAME_ENEMY_FLYING_RIGHT);

	#pragma endregion

	upgradeFont = CP_Font_Load("Assets/Fonts/Exo2-Regular.ttf");
	scoreFont = CP_Font_Load("Assets/Fonts/PressStart2P-Regular.ttf");

	white = CP_Color_Create(255, 255, 255, 255);
	darkGrey = CP_Color_Create(79, 79, 79, 255);
	black = CP_Color_Create(0, 0, 0, 255);
	brown = CP_Color_Create(65, 40, 4, 255);
	red = CP_Color_Create(255, 0, 0, 255);

	CP_Settings_RectMode(CP_POSITION_CENTER);

	gameState = GAME_STATE_PLAYING;
	gameTimer = 0;
	lastShotTimer = 0;
	lazerSpawnedTimer = 0;
	nextLazerTimer = 0;
	maxY = 0;
	playMusicOnce = 0;
	scoreMultipler = 1.0f;

	CP_Settings_Translate(0, 0);

	InitializePlayer(&player);
	InitializeBullets(bullets);
	InitializePickups(pickups);
	InitializeLazerHazard(lazerHazards);
	InitializeFlyingEnemies(flyingEnemies);
	InitializeWalkingEnemies(walkingEnemies);

	backgroundHeight1 = 512.0f;
	backgroundHeight2 = -512.0f;
	leftWallCoord = -192.0f;
	rightWallCoord = 1792.0f;

	#pragma region platforms

	wallWidth = (CP_System_GetWindowWidth() / 5.0f);		// The width of the side walls
	lastPlayerHeightThreshold = 9;							// The Y value the player has to pass to start generating non-starting platforms
	distanceBetweenPlatform = 2;							// Y distance between platforms (2 * 100 = 200)
	platformHeight = CP_System_GetWindowHeight() / 100.0f;	// Actual height of the platform

	// Generates the starting platforms of the game
	for (int i = 0; i < MAX_PLATFORMS; ++i) {
		platforms[i].y = (i * 200 + 500) / 100;
		GeneratePlatform(player, platforms, i, wallWidth);
	}

	#pragma endregion

}

void Game_Update(void) {
	gameTimer += CP_System_GetDt();

	if (playMusicOnce == 0) {
		playMusicOnce = 1;
		CP_Sound_PlayMusic(gameTrack);
	}
	
	#pragma region keyInputs

	wKeyPressed = CP_Input_KeyDown(KEY_W) || CP_Input_KeyDown(KEY_SPACE);
	wKeyTriggered = CP_Input_KeyTriggered(KEY_W) || CP_Input_KeyTriggered(KEY_SPACE);
	aKeyPressed = CP_Input_KeyDown(KEY_A);
	sKeyPressed = CP_Input_KeyDown(KEY_S);
	dKeyPressed = CP_Input_KeyDown(KEY_D);
	escKeyPressed = CP_Input_KeyTriggered(KEY_ESCAPE);				// on click, from low to high voltage
	leftMouseClick = CP_Input_MouseTriggered(MOUSE_BUTTON_LEFT);	// Assuming LEFT_BUTTON corresponds to the left mouse button
	leftMouseHold = CP_Input_MouseDown(MOUSE_BUTTON_LEFT);			// Left Click and Hold

	#pragma endregion

	#pragma region gameStateChecks

	if (escKeyPressed && gameState == GAME_STATE_PLAYING) { // Pause Screen
		gameState = GAME_STATE_PAUSE;
	} else if (maxY > FIRST_UPGRADE_Y_VALUE + (player.totalUpgrades * NEXT_UPGRADE_Y_VALUE) && (gameState != GAME_STATE_UPGRADE)) { // Upgrades Screen
		gameState = GAME_STATE_UPGRADE;
		maxY = player.y = 2000.0f + (player.totalUpgrades * 5000); // Set both maxY and playerY to upgrade constrain
		InitializeGameStateUpgrade(&gameStateUpgrade,player.x);
		Get3AvailableUpgrades(gameStateUpgrade.upgradeChoice, player.upgrades);
	} else if (player.currentHealth <= 0 && gameState == GAME_STATE_PLAYING) { // Defeat Screen
		gameState = GAME_STATE_DEFEAT;
		InitializeGameStateDefeat(&gameStateDefeat);
	}

	#pragma endregion

	#pragma region platformGeneration

	// Formatted Y value of the player
	// By making use of integer divion we are able to determine the players highest Y value in the 2s slightly shifted upwards (downwards on screen) by 1
	// (1357 / 100) / 2 * 2 + 1 = 13
	formattedPlayerY = ((int)player.y / 100) / distanceBetweenPlatform * distanceBetweenPlatform + 1;
	if (formattedPlayerY > lastPlayerHeightThreshold) {
		lastPlayerHeightThreshold = formattedPlayerY;
		int newPlatformY = (3 * distanceBetweenPlatform + lastPlayerHeightThreshold);
		ShiftPlatform(player, platforms, MAX_PLATFORMS, newPlatformY, wallWidth);
		DifficultyPlatformModifier(player.totalUpgrades, walkingEnemies, platforms);
	}

	#pragma endregion

	#pragma region Information Bar

	int depth = (int)maxY - 347;
	char depthText[MAX_SCORE_DIGITS];
	int depthSize = MAX_SCORE_DIGITS;
	snprintf(depthText, depthSize, "%07d", depth);

	int kills = player.killCount;
	char killsText[MAX_KILLS_DIGITS];
	int killsSize = MAX_KILLS_DIGITS;
	snprintf(killsText, killsSize, "%d", kills);
	
	if (kills % 10 == 0) {
		scoreMultipler = 1.0f + kills / 100.0f;
	}
	char multiplierText[MAX_KILLS_DIGITS];
	int multiplierSize = MAX_KILLS_DIGITS;
	snprintf(multiplierText, killsSize, "%.1fX", scoreMultipler);

	#pragma endregion

	#pragma region backgroundLogic

	if (player.y > backgroundHeight1)
		backgroundHeight2 = backgroundHeight1 + 1024.0f;
	if (player.y > backgroundHeight2)
		backgroundHeight1 = backgroundHeight2 + 1024.0f;

	#pragma endregion

	if (gameState == GAME_STATE_PLAYING) {

		#pragma region playerMovement

		if (player.verticalVelocity < 0) {
			// Jumping
			if (player.horizontalVelocity > 0) { // Moving Right
				player.imageAddress = GetAnimationImageAddress(playerJumpRight, MAX_FRAME_PLAYER_JUMP_RIGHT, gameTimer);
			} else if (player.horizontalVelocity < 0) { // Moving Left
				player.imageAddress = GetAnimationImageAddress(playerJumpLeft, MAX_FRAME_PLAYER_JUMP_LEFT, gameTimer);
			} else {
				player.imageAddress = GetAnimationImageAddress(playerJump, MAX_FRAME_PLAYER_JUMP, gameTimer);
			}
		} else {
			// Falling
			if (player.horizontalVelocity > 0) { // Moving Right
				player.imageAddress = GetAnimationImageAddress(playerFallRight, MAX_FRAME_PLAYER_FALL_RIGHT, gameTimer);
			} else if (player.horizontalVelocity < 0) { // Moving Left
				player.imageAddress = GetAnimationImageAddress(playerFallLeft, MAX_FRAME_PLAYER_FALL_LEFT, gameTimer);
			} else {
				player.imageAddress = GetAnimationImageAddress(playerFall, MAX_FRAME_PLAYER_FALL, gameTimer);
			}
		}
		// player.upgrades[3] -> Mid-Air Jumping
		if (isTouching == 1 || player.upgrades[3]) { // If jumping, no change in horizontal velocity allowed
			if (dKeyPressed && !aKeyPressed) { // Move Right
				player.horizontalVelocity = (float)player.movementSpeed;
				if (isTouching) {
					player.imageAddress = GetAnimationImageAddress(playerRight, MAX_FRAME_PLAYER_RIGHT, gameTimer);
				}
			} else if (aKeyPressed && !dKeyPressed) { // Move Left
				player.horizontalVelocity = -(float)player.movementSpeed;
				if (isTouching) {
					player.imageAddress = GetAnimationImageAddress(playerLeft, MAX_FRAME_PLAYER_LEFT, gameTimer);
				}
			} else {
				player.horizontalVelocity = 0;
				if (isTouching) {
					player.imageAddress = GetAnimationImageAddress(playerIdle, MAX_FRAME_PLAYER_IDLE, gameTimer);
				}
			}
		}

		#pragma endregion

		#pragma region collisionChecksAndUpdates

		if (wKeyPressed && player.upgrades[4] && !doubleJumpUsed && wKeyTriggered && !isTouching) {
			player.verticalVelocity = -(float)player.jumpSpeed; // Double JUMP! (This one cancels out pre existing gravity)
			doubleJumpUsed = 1;
			CP_Sound_Play(player.jumpSFX);
		}

		isTouching = 0; //set to touching nothing (Player is in air unless proven on the floor)
		isLeftOrRightOfPlatform = 1; // If this variable remains true: This frame is when player is same level as platform
		
		float leftPlayer = player.x - player.width / 2.0f;
		float rightPlayer = player.x + player.width / 2.0f;
		float topPlayer = player.y - player.height / 2.0f;
		float bottomPlayer = player.y + player.height / 2.0f;
		float horizontalFrameMovement = player.horizontalVelocity * CP_System_GetDt();
		float verticalFrameMovement = player.verticalVelocity * CP_System_GetDt();

		if (player.verticalVelocity > 0) { // Player is Decending
			for (int i = 0; i < 3; ++i) { // 3 Checks
				float leftPlatform1 = platforms[i].p1Coord - platforms[i].p1Length / 2.0f;
				float rightPlatform1 = platforms[i].p1Coord + platforms[i].p1Length / 2.0f;
				float leftPlatform2 = platforms[i].p2Coord - platforms[i].p2Length / 2.0f;
				float rightPlatform2 = platforms[i].p2Coord + platforms[i].p2Length / 2.0f;
				float topPlatform = platforms[i].y * 100 - platformHeight / 2.0f;
				float bottomPlatform = platforms[i].y * 100 + platformHeight / 2.0f;
				// Short Circut Check of !i to account for [0-1] subscript
				if ((!i || platforms[i - 1].y * 100 + platformHeight / 2.0f < topPlayer) && bottomPlayer < topPlatform) {
					isLeftOrRightOfPlatform = 0;
					if (player.horizontalVelocity > 0) { // player moving right and downwards
						if (leftPlayer > rightPlatform2) { // I am at right side moving right
							// Nothing to check, right wall will handle
							// Confirm Got nothing below player; Fall away!
							// Case of top right quadrant moving away from platform
						} else if (rightPlayer > leftPlatform2) { // directly on top of platform 2
							// Collision can happen with Platform 2
							// Case of top quadrant moving towards right
							// High Chance collision occur, normal top quad to middle
							// Low chance of collision negated, top quad moved to top right quad
							// Very Low chance of collision not captured by frame, top quad moved to right quad

							// Check if top quad become right / top right quad
							if (leftPlayer + horizontalFrameMovement > rightPlatform2) {
								// if top quad -> right quad or just top quad -> top right quad
								if (bottomPlayer + verticalFrameMovement > topPlatform) {// top quad moved to right quad
									if (IsPlayerStopped(player, 1, rightPlatform2, topPlatform)) {// Player landed on Platform 2
										player.verticalVelocity = 0;
										player.y = topPlatform - player.height / 2.0f;
										isTouching = 1;
									}
								}
							} else { // top quad is still top quad
								if (bottomPlayer + verticalFrameMovement > topPlatform) {
									player.verticalVelocity = 0;
									player.y = topPlatform - player.height / 2.0f;
									isTouching = 1;
								}
							}
						} else if (leftPlayer > rightPlatform1) { // Inbetween 2 Platforms
							// Collision can happen with Platform 2
							// Case of top left quadrant moving right (OR top right quad of platform1, moving right)
							// High Chance no collision, top left quad to top quad
							// Low chance no collision, top left quad to left quad
							// Low chance Scenario 1, top left quad to middle
							if (rightPlayer + horizontalFrameMovement > leftPlatform2) {
								if (bottomPlayer + verticalFrameMovement > topPlatform) {
									if (IsPlayerStopped(player, 1, leftPlatform2, topPlatform)) {
										// Player slam face first into platform 2 left wall
										player.horizontalVelocity = 0;
										player.x = leftPlatform2 - player.width / 2.0f;
									} else {
										// Player managed to land on platform 2
										player.verticalVelocity = 0;
										player.y = topPlatform - player.height / 2.0f;
										isTouching = 1;
									}
								}
								// Else, keep falling, heavnt hit platform yet
							}
							// Else u can fall whenever, below u got no platforms
						} else if (rightPlayer > leftPlatform1) { // directly on top of platform 1
							// Collision can happen with Platform 1 / 2 (almost never 2)
							// Case of top quadrant moving right
							// High Chance collision occur, normal top quad to middle
							// Low chance of collision negated, top quad moved to top right quad
							// Very Low chance of collision not captured by frame, top quad moved to right quad

							// && IsPlayerStopped(player, 1, rightPlatform1, topPlatform)

							// If he moved out of being above platform 1
							if (leftPlayer + horizontalFrameMovement > rightPlatform1) {
								if (bottomPlayer + verticalFrameMovement > topPlatform) {
									if (IsPlayerStopped(player, 1, rightPlatform1, topPlatform)) {
										// Player stopped on platform 1
										player.verticalVelocity = 0;
										player.y = topPlatform - player.height / 2.0f;
										isTouching = 1;
									}
									// Else he just kept falling
								}
								// Else, free fall time
							} else {
								if (bottomPlayer + verticalFrameMovement > topPlatform) {
									player.verticalVelocity = 0;
									player.y = topPlatform - player.height / 2.0f;
									isTouching = 1;
								}
							}
						} else { // Player is on the left of platform 1
							// Collision might happen with Platform 1
							// Case of top left quadrant moving right
							// High Chance no collision, top left quad to top quad
							// Low chance no collision, top left quad to left quad
							// Low chance Scenario 1, top left quad to middle

							// && IsPlayerStopped(player, 1, leftPlatform1, topPlatform)
							if (rightPlayer + horizontalFrameMovement > leftPlatform1) {
								// Player managed to move to above platform 1
								if (bottomPlayer + verticalFrameMovement > topPlatform) {
									// Player managed to also land into platform 1
									if (IsPlayerStopped(player, 1, leftPlatform1, topPlatform)) {
										// But Slammed face first into platform 1's left wall
										player.horizontalVelocity = 0;
										player.x = leftPlatform1 - player.width / 2.0f;
									} else {
										// And landed on top of platform 1 safely
										player.verticalVelocity = 0;
										player.y = topPlatform - player.height / 2.0f;
										isTouching = 1;
									}
								}
							}
						}
					} else if (player.horizontalVelocity < 0) { // player moving left and downwards
						if (rightPlayer < leftPlatform1) {
							// Fall Away
						} else if (leftPlayer < rightPlatform1) { // directly on top of platform 1
							// Collision can happen with Platform 1
							// Case of top quadrant moving towards left
							// High Chance collision occur, normal top quad to middle
							// Low chance of collision negated, top quad moved to top left quad
							// Very Low chance of collision not captured by frame, top quad moved to left quad

							// && IsPlayerStopped(player, 2, leftPlatform1, topPlatform)
							if (rightPlayer + horizontalFrameMovement < leftPlatform1) {
								if (bottomPlayer + verticalFrameMovement > topPlatform) {
									if (IsPlayerStopped(player, 2, leftPlatform1, topPlatform)) {
										// Player landed on platform 1 first
										player.verticalVelocity = 0;
										player.y = topPlatform - player.height / 2.0f;
										isTouching = 1;
									}
									// Player 1 escaped on left of platform 1
								}
							} else {
								// Check if he landed on platform 1
								if (bottomPlayer + verticalFrameMovement > topPlatform) {
									player.verticalVelocity = 0;
									player.y = topPlatform - player.height / 2.0f;
									isTouching = 1;
								}
							}
						} else if (rightPlayer < leftPlatform2) { // in between both platforms
							// Collision can happen with Platform 1
							// Case of top right quadrant moving left (OR top left quad of platform2, moving left)
							// High Chance no collision, top right quad to top quad
							// Low chance no collision, top right quad to right quad
							// Low chance Scenario 2, top right quad to middle

							// && IsPlayerStopped(player, 2, rightPlatform1, topPlatform)
							if (leftPlayer + horizontalFrameMovement < rightPlatform1) {
								if (bottomPlayer + verticalFrameMovement > topPlatform) {
									if (IsPlayerStopped(player, 2, rightPlatform1, topPlatform)) {
										// Face Planted on right wall of platform 1
										player.horizontalVelocity = 0;
										player.x = rightPlatform1 + player.width / 2.0f;
									} else {
										// Landed on top of platform 1
										player.verticalVelocity = 0;
										player.y = topPlatform - player.height / 2.0f;
										isTouching = 1;
									}
								}
							}
							// Continue to free fall, no platform below u
						} else if (leftPlayer < rightPlatform2) { // directly on top of platform 2
							// Collision can happen with Platform 1 / 2 (almost never 1)
							// Case of top quadrant moving left
							// High Chance collision occur, normal top quad to middle
							// Low chance of collision negated, top quad moved to top left quad
							// Very Low chance of collision not captured by frame, top quad moved to left quad

							// && IsPlayerStopped(player, 2, leftPlatform2, topPlatform)
							if (rightPlayer + horizontalFrameMovement < leftPlatform2) {
								if (bottomPlayer + verticalFrameMovement > topPlatform) {
									if (IsPlayerStopped(player, 2, leftPlatform2, topPlatform)) {
										// Landed on platform 2
										player.verticalVelocity = 0;
										player.y = topPlatform - player.height / 2.0f;
										isTouching = 1;
									}
									// Else, Player managed to escaped and theres is now no platforms below player
								}
							} else { // Continue falling towards the platform 2 
								if (bottomPlayer + verticalFrameMovement > topPlatform) {
									player.verticalVelocity = 0;
									player.y = topPlatform - player.height / 2.0f;
									isTouching = 1;
								}
							}
						} else { // Player is on the right of platform 2, moving left
							// Case of top right quadrant moving left
							// High Chance no collision, top right quad to top quad
							// Low chance no collision, top right quad to right quad
							// Low chance Scenario 1, top right quad to middle

							// && IsPlayerStopped(player, 2, rightPlatform2, topPlatform)
							if (leftPlayer + horizontalFrameMovement < rightPlatform2) {
								if (bottomPlayer + verticalFrameMovement > topPlatform) {
									// Hit Platform 2 but
									if (IsPlayerStopped(player, 2, rightPlatform2, topPlatform)) {
										// Stop at the right wall of platform 2
										player.horizontalVelocity = 0;
										player.x = rightPlatform2 + player.width / 2.0f;
									} else {
										// Landed on top of platform 2 
										player.verticalVelocity = 0;
										player.y = topPlatform - player.height / 2.0f;
										isTouching = 1;
									}
								}
							}
							// No platforms below player, keep falling
						}
					} else {
						// Will only match cases of top quadrant, falling into platform
						if (rightPlayer > leftPlatform1 && rightPlatform1 > leftPlayer) { // directly on top of platform 1
							if (bottomPlayer + verticalFrameMovement > topPlatform) {
								player.verticalVelocity = 0;
								player.y = topPlatform - player.height / 2.0f;
								isTouching = 1;
							}
						} else if (rightPlayer > leftPlatform2 && rightPlatform2 > leftPlayer) { // directly on top of platform 2
							if (bottomPlayer + verticalFrameMovement > topPlatform) {
								player.verticalVelocity = 0;
								player.y = topPlatform - player.height / 2.0f;
								isTouching = 1;
							}
						}
					}
					break;
				}
			}
		} else if (player.verticalVelocity < 0 && !player.upgrades[5]) { //if player is jumping (Ascending) || Climb Up used here
			for (int i = 0; i < 2; ++i) { // Only 2 Checks
				float leftPlatform1 = platforms[i].p1Coord - platforms[i].p1Length / 2.0f;
				float rightPlatform1 = platforms[i].p1Coord + platforms[i].p1Length / 2.0f;
				float leftPlatform2 = platforms[i].p2Coord - platforms[i].p2Length / 2.0f;
				float rightPlatform2 = platforms[i].p2Coord + platforms[i].p2Length / 2.0f;
				float topPlatform = platforms[i].y * 100 - platformHeight / 2.0f;
				float bottomPlatform = platforms[i].y * 100 + platformHeight / 2.0f;
				if (bottomPlatform < topPlayer && bottomPlayer < platforms[i + 1].y * 100 - platformHeight / 2.0f) { //if player is between platforms
					isLeftOrRightOfPlatform = 0;
					if (player.horizontalVelocity > 0) { // player moving right and upwards
						if (leftPlayer > rightPlatform2) { // I am at right side moving right
							// Nothing to check, right wall will handle
							// Confirm Got nothing above player; Jump away!
							// Case of bottom right quadrant moving away from platform
						} else if (rightPlayer > leftPlatform2) { // directly on below of platform 2
							// Collision can happen with Platform 2
							// Case of bottom quadrant moving towards right
							// High Chance collision occur, normal bottom quad to platform
							// Low chance of collision negated, bottom quad moved to bottom right quad
							// Very Low chance of collision not captured by frame, bottom quad moved to right quad

							// && IsPlayerStopped(player, 3, rightPlatform2, bottomPlatform)
							if (leftPlayer + horizontalFrameMovement > rightPlatform2) {
								// Player's X value escaped platform 2
								if (topPlayer + verticalFrameMovement < bottomPlatform) {
									if (IsPlayerStopped(player, 3, rightPlatform2, bottomPlatform)) {
										// Player head hit top of platform 2
										player.verticalVelocity = 0;
										player.y = bottomPlatform + player.height / 2.0f;
									}
									// Player Y also escaped, prolly jumped towards right wall
								}
							} else {
								// Player still below platform 2, might hit his head
								if (topPlayer + verticalFrameMovement < bottomPlatform) {
									player.verticalVelocity = 0;
									player.y = bottomPlatform + player.height / 2.0f;
								}
							}
						} else if (leftPlayer > rightPlatform1) { // Inbetween 2 Platforms
							// Collision can happen with Platform 2
							// Case of bottom left quadrant moving right (OR bottom right quad of platform1, moving right)
							// low Chance no collision, bottom left quad to bottom quad
							// High chance no collision, bottom left quad to left quad (Happen when jumping up)
							// Low chance Scenario 3, bottom left quad to platform (Bug HERE)

							// Checks if player's right movement caused it to go under / into platform 2
							if (rightPlayer + horizontalFrameMovement > leftPlatform2) {
								if (topPlayer + verticalFrameMovement < bottomPlatform) {
									if (IsPlayerStopped(player, 3, leftPlatform2, bottomPlatform)) {
										// Face Landed on Left side of platform 2, leaving a mark
										player.horizontalVelocity = 0;
										player.x = leftPlatform2 - player.width / 2.0f;
									} else {
										// Head smack on the bottom of platform 2, thats gotta sting
										player.verticalVelocity = 0;
										player.y = bottomPlatform + player.height / 2.0f;
									}
								}
							}
							// Nothing on top of player. Keep jumping boi
						} else if (rightPlayer > leftPlatform1) { // directly on top of platform 1
							// Collision can happen with Platform 1 / 2 (almost never 2)
							// Case of bottom quadrant moving right
							// High Chance collision occur, normal bottom quad to middle
							// Low chance of collision negated, bottom quad moved to bottom right quad
							// Very Low chance of collision not captured by frame, bottom quad moved to right quad

							// && IsPlayerStopped(player, 3, rightPlatform1, bottomPlatform)
							if (leftPlayer + horizontalFrameMovement > rightPlatform1) {
								// X value made it to a gap
								if (topPlayer + verticalFrameMovement < bottomPlatform) {
									// Y value smack his head
									if (IsPlayerStopped(player, 3, rightPlatform1, bottomPlatform)) {
										// Overall, player head has a bruise
										player.verticalVelocity = 0;
										player.y = bottomPlatform + player.height / 2.0f;
									}
									// Jump free my boi
								}
							} else {
								if (topPlayer + verticalFrameMovement < bottomPlatform) {
									player.verticalVelocity = 0;
									player.y = bottomPlatform + player.height / 2.0f;
								}
							}
						} else { // Collision might happen with Platform 1
							// Case of bottom left quadrant moving right
							// Low Chance no collision, bottom left quad to bottom quad
							// High chance no collision, bottom left quad to left quad
							// Low chance Scenario 3, bottom left quad to platform

							// Checks left quad to bottom quad
							if (rightPlayer + horizontalFrameMovement > leftPlatform1) {
								if (topPlayer + verticalFrameMovement < bottomPlatform) {
									if (IsPlayerStopped(player, 3, leftPlatform1, bottomPlatform)) {
										player.horizontalVelocity = 0;
										player.x = leftPlatform1 - player.width / 2.0f;
									} else {
										player.verticalVelocity = 0;
										player.y = bottomPlatform + player.height / 2.0f;
									}
								}
							}								
						}
					} else if (player.horizontalVelocity < 0) { // player direction is towards top left
						if (rightPlayer < leftPlatform1) {
							// Jump success, rest left wall will settle
						} else if (leftPlayer < rightPlatform1) { // directly on below of platform 1
							// Collision can happen with Platform 1
							// Case of bottom quadrant moving towards left
							// High Chance collision occur, normal bottom quad to middle
							// Low chance of collision negated, bottom quad moved to bottom left quad
							// Very Low chance of collision not captured by frame, bottom quad moved to left quad

							// && IsPlayerStopped(player, 4, leftPlatform1, bottomPlatform)
							if (rightPlayer + horizontalFrameMovement < leftPlatform1) {
								// Move out of platform 1, towards left (nothing)
								if (topPlayer + verticalFrameMovement < bottomPlatform) {
									if (IsPlayerStopped(player, 4, leftPlatform1, bottomPlatform)) {
										// BONK, head hit platform 1
										player.verticalVelocity = 0;
										player.y = bottomPlatform + player.height / 2.0f;
									} // Else, player made it to left side of platform 1
								}
							} else {
								// still under platform 1
								if (topPlayer + verticalFrameMovement < bottomPlatform) {
									player.verticalVelocity = 0;
									player.y = bottomPlatform + player.height / 2.0f;
								}
							}
						} else if (rightPlayer < leftPlatform2) { // in between both platforms
							// Collision can happen with Platform 1
							// Case of bottom right quadrant moving left (OR bottom left quad of platform2, moving left)
							// High Chance no collision, bottom right quad to top quad
							// Low chance no collision, bottom right quad to right quad
							// Low chance Scenario 4, bottom right quad to middle

							// && IsPlayerStopped(player, 4, rightPlatform1, bottomPlatform)
							if (leftPlayer + horizontalFrameMovement < rightPlatform1) {
								if (topPlayer + verticalFrameMovement < bottomPlatform) {
									if (IsPlayerStopped(player, 4, rightPlatform1, bottomPlatform)) {
										// Slam face on Right side of platform 1
										player.horizontalVelocity = 0;
										player.x = rightPlatform1 + player.width / 2.0f;
									} else {
										// Head hit plat form 1
										player.verticalVelocity = 0;
										player.y = bottomPlatform + player.height / 2.0f;
									}
								}
							}
						} else if (leftPlayer < rightPlatform2) { // directly on top of platform 2
							// Collision can happen with Platform 1 / 2 (almost never 1)
							// Case of bottom quadrant moving left
							// High Chance collision occur, normal bottom quad to middle
							// Low chance of collision negated, bottom quad moved to bottom left quad
							// Very Low chance of collision not captured by frame, bottom quad moved to left quad

							// && IsPlayerStopped(player, 4, leftPlatform2, bottomPlatform)
							if (rightPlayer + horizontalFrameMovement < leftPlatform2) {
								if (topPlayer + verticalFrameMovement < bottomPlatform) {
									if (IsPlayerStopped(player, 4, leftPlatform2, bottomPlatform)) {
										// bonk on platform 2
										player.verticalVelocity = 0;
										player.y = bottomPlatform + player.height / 2.0f;
									}
									// else, player manages to escape between platform 1 and 2
								}
							} else {
								// Still under platform 2
								if (topPlayer + verticalFrameMovement < bottomPlatform) {
									player.verticalVelocity = 0;
									player.y = bottomPlatform + player.height / 2.0f;
								}
							}
						} else {
							// Case of bottom right quadrant moving left
							// High Chance no collision, bottom right quad to top quad
							// Low chance no collision, bottom right quad to right quad
							// Low chance Scenario 4, bottom right quad to middle

							// && IsPlayerStopped(player, 4, rightPlatform2, bottomPlatform)
							if (leftPlayer + horizontalFrameMovement < rightPlatform2) {
								if (topPlayer + verticalFrameMovement < bottomPlatform) {
									if (IsPlayerStopped(player, 4, rightPlatform2, bottomPlatform)) {
										player.horizontalVelocity = 0;
										player.x = rightPlatform2 + player.width / 2.0f;
									} else {
										player.verticalVelocity = 0;
										player.y = bottomPlatform + player.height / 2.0f;
									}
								}
							}
							// Else just jump only, no collision
						}
					} else {
						// Will only match cases of top quadrant, jumping head first into platform
						if (rightPlayer > leftPlatform1 && rightPlatform1 > leftPlayer) { // directly on below of platform 1
							if (topPlayer + verticalFrameMovement < bottomPlatform) {
								player.verticalVelocity = 0;
								player.y = bottomPlatform + player.height / 2.0f;
							}
						} else if (rightPlayer > leftPlatform2 && rightPlatform2 > leftPlayer) { // directly on below of platform 2
							if (topPlayer + verticalFrameMovement < bottomPlatform) {
								player.verticalVelocity = 0;
								player.y = bottomPlatform + player.height / 2.0f;
							}
						}
					}
					break;
				}
			}
		} else if (player.verticalVelocity == 0) { 
			// This case will happen right after player hit head on roof
			// For Now, Lets ignore this case
			// BUG SPOTTED: with double jump, player can jump up platform
		}

		if (IsCeilingCollided(player.y + verticalFrameMovement, player.height, maxY)) {
			player.y = maxY - CP_System_GetWindowHeight() / 2.0f + player.height / 2.0f;
			player.verticalVelocity = 0;
		}

		if (IsRightWallCollided(player.x + horizontalFrameMovement, player.width, wallWidth)) {
			player.x = (CP_System_GetWindowWidth() - wallWidth) - player.width / 2.0f;
			player.horizontalVelocity = 0;
		} else if (IsLeftWallCollided(player.x + horizontalFrameMovement, player.width, wallWidth)) {
			player.x = wallWidth + player.width / 2.0f;
			player.horizontalVelocity = 0;
		}

		if (isLeftOrRightOfPlatform) {
			for (int i = 2; i >= 0; --i) { // 3 Times
				float leftPlatform1 = platforms[i].p1Coord - platforms[i].p1Length / 2.0f;
				float rightPlatform1 = platforms[i].p1Coord + platforms[i].p1Length / 2.0f;
				float leftPlatform2 = platforms[i].p2Coord - platforms[i].p2Length / 2.0f;
				float rightPlatform2 = platforms[i].p2Coord + platforms[i].p2Length / 2.0f;
				float topPlatform = platforms[i].y * 100 - platformHeight / 2.0f;
				float bottomPlatform = platforms[i].y * 100 + platformHeight / 2.0f;
				if ((topPlatform < bottomPlayer) && !player.upgrades[5]) {
					// Found the correct Platform to check against
					if (player.horizontalVelocity > 0) { // Moving Right
						if (leftPlayer >= rightPlatform2) { // Right of Platform 2
							// Nothing to check, right wall will handle
						} else if (rightPlayer == leftPlatform2) { // Touching Platform 2
							// No right movement allowed
							player.horizontalVelocity = 0;
						} else if (rightPlayer > leftPlatform2 && !fallingThroughPlatform) { // "Inside" of Platform 2
							// Wont'happen, but if it does, can cast exception here
							// Casting Exception to throw player above platform 2
							player.y = topPlatform - player.height / 2.0f;
							player.verticalVelocity = 0;
							isTouching = 1;
						} else if (rightPlayer > leftPlatform2) { // Just to ensure the "Else if chain" is correctly implemented
							// Wont'happen, but if it does, can cast exception here
						} else if (leftPlayer >= rightPlatform1) { // Inbetween 2 Platforms
							if (rightPlayer + horizontalFrameMovement > leftPlatform2) {
								player.horizontalVelocity = 0;
								player.x = leftPlatform2 - player.width / 2.0f;
							}
						} else if (rightPlayer == leftPlatform1) { // Touching Platform 1
							// No right movement allowed
							player.horizontalVelocity = 0;
						} else if (rightPlayer > leftPlatform1 && !fallingThroughPlatform) { // "Inside" of Platform 1
							// Wont'happen, but if it does, can cast exception here
							player.y = topPlatform - player.height / 2.0f;
							player.verticalVelocity = 0;
							isTouching = 1;
						} else if (rightPlayer > leftPlatform1) { // Just to ensure the "Else if chain" is correctly implemented
							// Wont'happen, but if it does, can cast exception here
						} else { // Left of Platform 1
							if (rightPlayer + horizontalFrameMovement > leftPlatform1) {
								player.horizontalVelocity = 0;
								player.x = leftPlatform1 - player.width / 2.0f;
							}
						}
					} else if (player.horizontalVelocity < 0) { // Moving left
						if (rightPlayer <= leftPlatform1) { // Left Most of Screen
							// Jump success, rest left wall will settle
						} else if (leftPlayer == rightPlatform1) { // Touching Platform 1
							// No right movement allowed
							player.horizontalVelocity = 0;
						} else if (leftPlayer < rightPlatform1 && !fallingThroughPlatform) { // Inside Platform 1
							// Wont'happen, but if it does, can cast exception here
							player.y = topPlatform - player.height / 2.0f;
							player.verticalVelocity = 0;
							isTouching = 1;
						} else if (leftPlayer < rightPlatform1) {// Just to ensure the "Else if chain" is correctly implemented
							// Wont'happen, but if it does, can cast exception here
						} else if (rightPlayer <= leftPlatform2) { // in between both platforms
							if (leftPlayer + horizontalFrameMovement < rightPlatform1) {
								player.horizontalVelocity = 0;
								player.x = rightPlatform1 + player.width / 2.0f;
							}
						} else if (leftPlayer == rightPlatform2) { // Touching Platform 2
							// No right movement allowed
							player.horizontalVelocity = 0;
						} else if (leftPlayer < rightPlatform2 && !fallingThroughPlatform) {
							// Wont'happen, but if it does, can cast exception here
							player.y = topPlatform - player.height / 2.0f;
							player.verticalVelocity = 0;
							isTouching = 1;
						} else if (leftPlayer < rightPlatform2) {// Just to ensure the "Else if chain" is correctly implemented
							// Wont'happen, but if it does, can cast exception here
						} else {
							if (leftPlayer + horizontalFrameMovement < rightPlatform2) {
								player.horizontalVelocity = 0;
								player.x = rightPlatform2 + player.width / 2.0f;
							}
						}
					}
					break;
				} else if ((topPlatform == bottomPlayer)) {
					if (rightPlayer > leftPlatform1 && rightPlatform1 > leftPlayer) { // directly on below of platform 1
						player.verticalVelocity = 0;
						isTouching = 1; // My player's Just Standing, Chilling man
					} else if (rightPlayer > leftPlatform2 && rightPlatform2 > leftPlayer) { // directly on below of platform 2
						player.verticalVelocity = 0;
						isTouching = 1; // My player's Just Standing, Chilling man
					}
					break;
				}
			}
		}

		// Gravity
		if (isTouching == 0 && gameTimer >= 1.0f) { // Falling
			player.verticalVelocity += GRAVITY * CP_System_GetDt();
		} else if (player.upgrades[6] && player.verticalVelocity == 0 && sKeyPressed) { // Climb Down
			fallingThroughPlatform = 1;
			player.verticalVelocity += GRAVITY * CP_System_GetDt();
		} else {
			fallingThroughPlatform = 0;
			doubleJumpUsed = 0;
		}

		if (wKeyPressed && isTouching) { // Want to Jump and is on the floor
			CP_Sound_Play(player.jumpSFX);
			player.verticalVelocity -= player.jumpSpeed; // Jump
		}

		player.x += player.horizontalVelocity * CP_System_GetDt();
		player.y += (player.verticalVelocity * CP_System_GetDt()) > TERMINAL_VELOCITY ? TERMINAL_VELOCITY : (player.verticalVelocity * CP_System_GetDt());
		maxY = maxY < player.y ? player.y : maxY; // Check if current player y value is larger than highest y value the player last went
		// IMPORTANT!!! After this point, NO MORE UPDATING OF PLAYER X / Y VALUE

		#pragma endregion	

		#pragma region pickups

		if (CheckPlayerPickedUpHealth(player, pickups)) {
			player.currentHealth += 1; // Heal 1 Health/Pickup
			CP_Sound_Play(player.pickupSFX);
		}

		DespawnHealthPickup(pickups, maxY);

		#pragma endregion

		#pragma region hazards

		for (int i = 0; i < MAX_LAZER_HAZARDS; ++i) {
			if (lazerHazards[i].isActive && lazerHazards[i].timeCreated + lazerHazards[i].timeToFire < gameTimer) {
				DidLazerHitPlayer(&player, &lazerHazards[i]);
			}
		}

		#pragma endregion

		#pragma region bullets

		float mouseX = CP_Input_GetMouseWorldX(); // In our game, GetMouseX == GetMouseWorldX
		float mouseY = CP_Input_GetMouseWorldY();

		// Handle Bullet Firing
		if (leftMouseHold) {
			if (1.0f / (float)player.fireRate < gameTimer - lastShotTimer) {
				FireBullet(bullets, player, mouseX, mouseY);
				lastShotTimer = gameTimer;
			}
		}

		// Bullet Platform Collision
		if (!player.upgrades[7]) {
			BulletHitPlatform(bullets, platforms, platformHeight);
		}

		// Moves / Update Bullets
		UpdateBullets(bullets, maxY, wallWidth);
		BulletHitWalkingEnemy(bullets, walkingEnemies, &player, pickups);
		BulletHitFlyingEnemy(bullets, flyingEnemies, &player); // Bullet Hitting & Damaging Enemy

		#pragma endregion

		#pragma region enemySpawning

		DifficultyModifier(player.totalUpgrades, flyingEnemies, lazerHazards, maxY, gameTimer, &lastSpawnTimer, &lazerSpawnedTimer, &nextLazerTimer);

		//Walking Enemy Logic
		DespawnWalkingEnemy(walkingEnemies, maxY);
		WalkingEnemyLogic(player, walkingEnemies);
		WalkingEnemyTouchPlayer(walkingEnemies, &player);

		//Flying Enemy Logic
		DespawnFlyingEnemy(flyingEnemies, maxY);
		UpdateFlyingEnemies(player, flyingEnemies); // Handles the moving of flying enemies
		FlyingEnemyTouchPlayer(flyingEnemies, &player); // Handles the collision and damage of the player

		#pragma endregion

		#pragma region graphics

		CP_Graphics_ClearBackground(white);

		// Camera Movement
		CP_Settings_Translate(0, -(maxY - CP_System_GetWindowHeight() / 2.0f));

		// Draw Background
		CP_Image_Draw(mainBackgroundImage, CP_System_GetWindowWidth() / 2.0f, backgroundHeight1, 2048, 1024, 255);
		CP_Image_Draw(mainBackgroundImage, CP_System_GetWindowWidth() / 2.0f, backgroundHeight2, 2048, 1024, 255);

		// Draw Background Conjoin Lines
		CP_Settings_Stroke(brown);
		CP_Graphics_DrawLine(0.0f, backgroundHeight1 + 512.0f, 1600.0f, backgroundHeight1 + 512.0f);
	 	CP_Graphics_DrawLine(0.0f, backgroundHeight2 + 512.0f, 1600.0f, backgroundHeight2 + 512.0f);

		// Draw tutorial controls
		CP_Image_Draw(tutorial_playerControls, CP_System_GetWindowWidth() / 3.0f, 300, 225, 225, 255); // Left, at start of game
		CP_Image_Draw(tutorial_playerUpgrades, CP_System_GetWindowWidth() / 2.0f, 650, 225, 225, 255); // Centre, at start of game
		CP_Image_Draw(tutorial_playerShooting, (CP_System_GetWindowWidth() / 3.0f) * 2, 300, 225, 225, 255); // Right, at start of game
		CP_Image_Draw(tutorial_flyingEnemiesWarning, CP_System_GetWindowWidth() / 2.0f, 7900, 350, 75, 255); // Centre, after 2 upgrades
		CP_Image_Draw(tutorial_laserWarning, CP_System_GetWindowWidth() / 2.0f, 17900, 350, 75, 255); // Centre, after 4 upgrades

		// Draw Side Walls
		CP_Image_Draw(brickSideWalls, -192.0f, backgroundHeight1, 1024, 1024, 255);
		CP_Image_Draw(brickSideWalls, -192.0f, backgroundHeight2, 1024, 1024, 255);
		CP_Image_Draw(brickSideWalls, 1792.0f, backgroundHeight1, 1024, 1024, 255);
		CP_Image_Draw(brickSideWalls, 1792.0f, backgroundHeight2, 1024, 1024, 255);

		// Draw Side Wall Conjoin Lines
		CP_Settings_Stroke(darkGrey);
		CP_Graphics_DrawLine(0.0f, backgroundHeight1 + 512.0f, 320.0f, backgroundHeight1 + 512.0f);
		CP_Graphics_DrawLine(0.0f, backgroundHeight2 + 512.0f, 320.0f, backgroundHeight2 + 512.0f);
		CP_Graphics_DrawLine(1280.0f, backgroundHeight1 + 512.0f, 1600.0f, backgroundHeight1 + 512.0f);
		CP_Graphics_DrawLine(1280.0f, backgroundHeight2 + 512.0f, 1600.0f, backgroundHeight2 + 512.0f);

		// Draw Platforms
		DrawPlatforms(platforms, MAX_PLATFORMS, platformHeight);

		// Draw Info Bar
		CP_Font_Set(scoreFont);
		CP_Settings_Fill(white);
		CP_Settings_TextSize(30.0f);
		CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, CP_TEXT_ALIGN_V_MIDDLE);
		CP_Font_DrawText("DEPTH", CP_System_GetWindowWidth() - 5.0f, maxY - 375);
		CP_Font_DrawText(depthText, CP_System_GetWindowWidth() - 5.0f, maxY - 340);
		CP_Font_DrawText("KILLS", CP_System_GetWindowWidth() - 5.0f, maxY - 290);
		CP_Font_DrawText(killsText, CP_System_GetWindowWidth() - 5.0f, maxY - 255);
		CP_Font_DrawText("MULTIPLIER", CP_System_GetWindowWidth() - 5.0f, maxY - 210);
		CP_Font_DrawText(multiplierText, CP_System_GetWindowWidth() - 5.0f, maxY - 175);

		// Draw Hazards 
		DrawLazerHazard(lazerHazards, gameTimer, wallWidth);

		// Draw Bullets
		DrawBullets(bullets, bulletImage);
		DrawBullets(bullets, bulletImage);

		// Draw Pickups
		AnimatePickupHealths(pickups, pickupsHealth, gameTimer);
		DrawPickupHealth(pickups);

		// Draw Player
		CP_Image_Draw(*GetAnimationImageAddress(playerCrystal, MAX_FRAME_PLAYER_CRYSTAL, gameTimer), player.x, player.y - (player.height + player.width) / 2.0f, player.width, player.width, 255);
		CP_Image_Draw(*player.imageAddress, player.x, player.y, player.width, player.height, 255);

		// Draw Enemy
		DrawFlyingEnemies(flyingEnemies, player, ememyFlyingLeft, ememyFlyingRight, gameTimer); // Handles the drawing of Fying Enemy
		DrawWalkingEnemies(walkingEnemies, ememyWalkingLeft, ememyWalkingRight, gameTimer);

		// Draw UIs
		float shiftDown = maxY - CP_System_GetWindowHeight() / 2.0f; // shiftdown is the y value of the top of the camera.
		DrawHeatlhUI(healthEmptyUI, healthFullUI, player.currentHealth, player.maxHealth, 50.0f, 150 + shiftDown, wallWidth);
		DrawUpgradeUI(upgradeIcon, player.upgrades, 75.0f, 400 + shiftDown, wallWidth);

		// Draw Health Screen
		DrawDamagedView(player, maxY, damagedScreen);

		#pragma endregion

	} else if (gameState == GAME_STATE_UPGRADE) {

		#pragma region gameStateUpgrades

		player.horizontalVelocity = 0; // To make player always fall to the center
		// Upgrade Game State goes like this
		// Right after Y:2000 -> Animation before Upgrade choice -> Upgrade Choice -> Animation after Upgrade Choice -> Continue Game
		float timeBeforeUpgrade = 1.5f;
		float timeBeforeEndState = 1.5f;
		int rng1 = gameStateUpgrade.upgradeChoice[0];
		int rng2 = gameStateUpgrade.upgradeChoice[1];
		int rng3 = gameStateUpgrade.upgradeChoice[2];
		int selectedUpgradeElement;

		if (!gameStateUpgrade.isUpgradeChosen) { // PART 1: When the Player hasn't chosen Upgrade
			gameStateUpgrade.secondsSinceState += CP_System_GetDt();
			gameStateUpgrade.chosenUpgrade = GetUpgradeSelected(maxY);			

			if (gameStateUpgrade.chosenUpgrade && leftMouseClick && (gameStateUpgrade.secondsSinceState / timeBeforeUpgrade > 1)) { // Player Clicked Upgrade
				player.upgrades[gameStateUpgrade.upgradeChoice[gameStateUpgrade.chosenUpgrade - 1]]++;
				if (gameStateUpgrade.upgradeChoice[gameStateUpgrade.chosenUpgrade - 1] == 0) {
					player.maxHealth++;
					player.currentHealth++;
				} else if (gameStateUpgrade.upgradeChoice[gameStateUpgrade.chosenUpgrade - 1] == 1) {
					player.damage += UPGRADE_DAMAGE_INCREMENT;
				} else if (gameStateUpgrade.upgradeChoice[gameStateUpgrade.chosenUpgrade - 1] == 2) {
					player.movementSpeed += UPGRADE_SPEED_INCREMENT;
				} else if (gameStateUpgrade.upgradeChoice[gameStateUpgrade.chosenUpgrade - 1] == 8) {
					player.fireRate += UPGRADE_FIRERATE_INCREMENT;
				} else if (gameStateUpgrade.upgradeChoice[gameStateUpgrade.chosenUpgrade - 1] == 9) {
					player.projectileSize += UPGRADE_PROJECTILESIZE_INCREMENT;
				} else if (gameStateUpgrade.upgradeChoice[gameStateUpgrade.chosenUpgrade - 1] == 10) {
					player.projectileSpeed += UPGRADE_PROJECTILESPEED_INCREMENT;
				}
				player.totalUpgrades++;
				player.verticalVelocity = 0;
				gameStateUpgrade.isUpgradeChosen = 1;
				InitializeLazerHazard(lazerHazards);
				InitializeFlyingEnemies(flyingEnemies);
				InitializeWalkingEnemies(walkingEnemies);
				InitializeBullets(bullets);
			}
		}
		// NOTE: I know it looks like it should be a "else". 
		// But for that specific frame that upgrade is clicked, both if statements need to go through
		if (gameStateUpgrade.isUpgradeChosen) { // PART 2: After the Player chosen upgrade
			gameStateUpgrade.secondsSinceUpgradeChosen += CP_System_GetDt();
			selectedUpgradeElement = gameStateUpgrade.upgradeChoice[gameStateUpgrade.chosenUpgrade - 1];

			if (gameStateUpgrade.secondsSinceUpgradeChosen > timeBeforeEndState) { // Animations are done, Next Phase
				gameState = GAME_STATE_PLAYING;
			}
		}

		#pragma endregion

		#pragma region graphics

		CP_Graphics_ClearBackground(white);

		// Camera Movement
		CP_Settings_Translate(0, -(maxY - CP_System_GetWindowHeight() / 2.0f));

		// Draw Background
		CP_Image_Draw(mainBackgroundImage, CP_System_GetWindowWidth() / 2.0f, backgroundHeight1, 2048, 1024, 255);
		CP_Image_Draw(mainBackgroundImage, CP_System_GetWindowWidth() / 2.0f, backgroundHeight2, 2048, 1024, 255);

		// Draw Background Conjoin Lines
		CP_Settings_Stroke(brown);
		CP_Graphics_DrawLine(0.0f, backgroundHeight1 + 512.0f, 1600.0f, backgroundHeight1 + 512.0f);
		CP_Graphics_DrawLine(0.0f, backgroundHeight2 + 512.0f, 1600.0f, backgroundHeight2 + 512.0f);

		// Draw Platforms
		DrawPlatforms(platforms, MAX_PLATFORMS, platformHeight);

		if (!gameStateUpgrade.isUpgradeChosen) { // PART 1: When the Player hasn't chosen Upgrade
			float animationTimer = gameStateUpgrade.secondsSinceState / timeBeforeUpgrade > 1 ? 1 : gameStateUpgrade.secondsSinceState / timeBeforeUpgrade;
			float outgoingSideWalls = wallWidth * animationTimer; // Timer goes from 0% to 100%
			
			// Draw Side Walls
			CP_Image_Draw(brickSideWalls, -192.0f - outgoingSideWalls, backgroundHeight1, 1024, 1024, 255);
			CP_Image_Draw(brickSideWalls, -192.0f - outgoingSideWalls, backgroundHeight2, 1024, 1024, 255);
			CP_Image_Draw(brickSideWalls, 1792.0f + outgoingSideWalls, backgroundHeight1, 1024, 1024, 255);
			CP_Image_Draw(brickSideWalls, 1792.0f + outgoingSideWalls, backgroundHeight2, 1024, 1024, 255);

			// Draw Side Wall Conjoin Lines
			CP_Settings_Stroke(darkGrey);
			CP_Graphics_DrawLine(0.0f, backgroundHeight1 + 512.0f, 320.0f - outgoingSideWalls, backgroundHeight1 + 512.0f);
			CP_Graphics_DrawLine(0.0f, backgroundHeight2 + 512.0f, 320.0f - outgoingSideWalls, backgroundHeight2 + 512.0f);
			CP_Graphics_DrawLine(1280.0f + outgoingSideWalls, backgroundHeight1 + 512.0f, 1600.0f, backgroundHeight1 + 512.0f);
			CP_Graphics_DrawLine(1280.0f + outgoingSideWalls, backgroundHeight2 + 512.0f, 1600.0f, backgroundHeight2 + 512.0f);

			player.x = CP_System_GetWindowWidth() / 2.0f + gameStateUpgrade.distanceToCenter * (1 - animationTimer);
			maxY = player.y = FIRST_UPGRADE_Y_VALUE + NEXT_UPGRADE_Y_VALUE * player.totalUpgrades + FALLING_Y_VALUE / 2.0f * (animationTimer);

			CP_Image_Draw(playerFall[0], player.x, player.y, player.width, player.height, 255);

			// Draw Health Screen
			DrawDamagedView(player, maxY, damagedScreen);

			DrawUpgradeSelection(
				upgradeIcon[rng1], upgrades[rng1].upgradeName, upgrades[rng1].upgradeText,
				upgradeIcon[rng2], upgrades[rng2].upgradeName, upgrades[rng2].upgradeText,
				upgradeIcon[rng3], upgrades[rng3].upgradeName, upgrades[rng3].upgradeText,
				upgradeFont, maxY, gameStateUpgrade.secondsSinceState, timeBeforeUpgrade, gameStateUpgrade.chosenUpgrade
			);
		} else { // PART 2: After the Player chosen Upgrade
			float animationTimer = gameStateUpgrade.secondsSinceUpgradeChosen / timeBeforeUpgrade > 1 ? 1 : gameStateUpgrade.secondsSinceUpgradeChosen / timeBeforeUpgrade;
			float incomingSideWalls = wallWidth * animationTimer; // Timer goes from 0% to 100%
			
			// Draw Side Walls
			CP_Image_Draw(brickSideWalls, -512.0f + incomingSideWalls, backgroundHeight1, 1024, 1024, 255);
			CP_Image_Draw(brickSideWalls, -512.0f + incomingSideWalls, backgroundHeight2, 1024, 1024, 255);
			CP_Image_Draw(brickSideWalls, 2112.0f - incomingSideWalls, backgroundHeight1, 1024, 1024, 255);
			CP_Image_Draw(brickSideWalls, 2112.0f - incomingSideWalls, backgroundHeight2, 1024, 1024, 255);

			// Draw Side Wall Conjoin Lines
			CP_Settings_Stroke(darkGrey);
			CP_Graphics_DrawLine(0.0f, backgroundHeight1 + 512.0f, 0.0f + incomingSideWalls, backgroundHeight1 + 512.0f);
			CP_Graphics_DrawLine(0.0f, backgroundHeight2 + 512.0f, 0.0f + incomingSideWalls, backgroundHeight2 + 512.0f);
			CP_Graphics_DrawLine(1600.0f - incomingSideWalls, backgroundHeight1 + 512.0f, 1600.0f, backgroundHeight1 + 512.0f);
			CP_Graphics_DrawLine(1600.0f - incomingSideWalls, backgroundHeight2 + 512.0f, 1600.0f, backgroundHeight2 + 512.0f);

			CP_Image_Draw(playerFall[0], player.x, player.y, player.width, player.height, 255);

			// Draw Health Screen
			DrawDamagedView(player, maxY, damagedScreen);

			DrawAfterUpgradeSelection(
				upgradeIcon[selectedUpgradeElement], upgrades[selectedUpgradeElement].upgradeName, upgrades[selectedUpgradeElement].upgradeText,
				upgradeFont, maxY, gameStateUpgrade.secondsSinceUpgradeChosen, timeBeforeEndState, gameStateUpgrade.chosenUpgrade
			);
		}

		#pragma endregion

	} else if (gameState == GAME_STATE_PAUSE) {
		CP_Graphics_ClearBackground(white);

		CP_Settings_Translate(0, -(maxY - CP_System_GetWindowHeight() / 2.0f));

		int buttonSelected = GetPauseButtonSelected(maxY);
		
		// Draw Background
		CP_Image_Draw(mainBackgroundImage, CP_System_GetWindowWidth() / 2.0f, backgroundHeight1, 2048, 1024, 255);
		CP_Image_Draw(mainBackgroundImage, CP_System_GetWindowWidth() / 2.0f, backgroundHeight2, 2048, 1024, 255);

		// Draw Background Conjoin Lines
		CP_Settings_Stroke(brown);
		CP_Graphics_DrawLine(0.0f, backgroundHeight1 + 512.0f, 1600.0f, backgroundHeight1 + 512.0f);
		CP_Graphics_DrawLine(0.0f, backgroundHeight2 + 512.0f, 1600.0f, backgroundHeight2 + 512.0f);

		// Draw Pause Menu
		DrawPauseMenu(maxY, buttonSelected);
		
		// Draw Health Screen
		DrawDamagedView(player, maxY, damagedScreen);

		if (buttonSelected == 1 && leftMouseClick) { // Resume
			gameState = GAME_STATE_PLAYING;
		} else if (buttonSelected == 2 && leftMouseClick) { // Main Menu
			CP_Sound_StopGroup(CP_SOUND_GROUP_1);
			CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
		} else if (buttonSelected == 3 && leftMouseClick) { // Quit
			CP_Sound_StopGroup(CP_SOUND_GROUP_1);
			CP_Engine_Terminate();
		}
	} else if (gameState == GAME_STATE_DEFEAT) {
		gameStateDefeat.secondsSinceState += CP_System_GetDt();

		// Defeat Game State goes like this
		// player.currentHealth == 0 -> Animation of Player -> Animation of Walls Closing -> Text Appear
		CP_Image *playerAnimation = NULL;
		float timeForPlayer = 1;
		float timeForWallClose = 1;
		float leftWallClosingCoord = leftWallCoord;
		float rightWallClosingCoord = rightWallCoord;
		int isWallClosed;
		int buttonSelected = 0;

		for (int i = MAX_FRAME_PLAYER_DEFEAT - 1; i >= 0; --i) { // Animate Through each frame of the player
			if (gameStateDefeat.secondsSinceState > (timeForPlayer / MAX_FRAME_PLAYER_DEFEAT) * i) {
				playerAnimation = &playerDefeat[i];
				break;
			} else {
				playerAnimation = &playerDefeat[0];
			}
		}

		if (gameStateDefeat.secondsSinceState < timeForPlayer) { // Side Walls are open
			isWallClosed = 0;
		} else if (gameStateDefeat.secondsSinceState < timeForPlayer + timeForWallClose) { // Animations of Side Walls closing
			float spaceToCover = CP_System_GetWindowWidth() / 2.0f - wallWidth;
			float animationTimer = (gameStateDefeat.secondsSinceState - timeForPlayer) / timeForWallClose > 1 ? 1 : (gameStateDefeat.secondsSinceState - timeForPlayer) / timeForWallClose;
			leftWallClosingCoord = leftWallCoord + spaceToCover * (animationTimer);
			rightWallClosingCoord = rightWallCoord - spaceToCover * (animationTimer);
			isWallClosed = 0;
		} else { // Side Walls are closed
			isWallClosed = 1;
			leftWallClosingCoord = 288.0f;
			rightWallClosingCoord = 1312.0f;
			buttonSelected = GetDefeatButtonSelected(maxY);
			if (leftMouseClick && buttonSelected == 1) {
				CP_Sound_StopGroup(CP_SOUND_GROUP_1);
				CP_Engine_SetNextGameState(Restart_Init, Restart_Update, Restart_Exit);
			} else if (leftMouseClick && buttonSelected == 2) {
				CP_Sound_StopGroup(CP_SOUND_GROUP_1);
				CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
			}
		}

		#pragma region graphics

		CP_Graphics_ClearBackground(white);

		// Camera Movement
		CP_Settings_Translate(0, -(maxY - CP_System_GetWindowHeight() / 2.0f));

		// Draw Background
		CP_Image_Draw(mainBackgroundImage, CP_System_GetWindowWidth() / 2.0f, backgroundHeight1, 2048, 1024, 255);
		CP_Image_Draw(mainBackgroundImage, CP_System_GetWindowWidth() / 2.0f, backgroundHeight2, 2048, 1024, 255);

		// Draw Background Conjoin Lines
		CP_Settings_Stroke(brown);
		CP_Graphics_DrawLine(0.0f, backgroundHeight1 + 512.0f, 1600.0f, backgroundHeight1 + 512.0f);
		CP_Graphics_DrawLine(0.0f, backgroundHeight2 + 512.0f, 1600.0f, backgroundHeight2 + 512.0f);

		// Draw Plaforms
		DrawPlatforms(platforms, MAX_PLATFORMS, platformHeight);

		// Draw Player Death Animation
		CP_Image_Draw(*playerAnimation, player.x, player.y, player.width, player.height, 255);

		if (isWallClosed) { // Walls are fully closed
			CP_Settings_Fill(black);
			CP_Graphics_DrawRect(CP_System_GetWindowWidth() / 2.0f, maxY, (float)CP_System_GetWindowWidth(), (float)CP_System_GetWindowHeight());
			DrawDefeatText(maxY, scoreMultipler, buttonSelected);
		} else { // Walls are open and during the closing animation
			// Draw Side Walls
			CP_Image_Draw(brickSideWalls, leftWallClosingCoord, backgroundHeight1, 1024, 1024, 255);
			CP_Image_Draw(brickSideWalls, leftWallClosingCoord, backgroundHeight2, 1024, 1024, 255);
			CP_Image_Draw(brickSideWalls, rightWallClosingCoord, backgroundHeight1, 1024, 1024, 255);
			CP_Image_Draw(brickSideWalls, rightWallClosingCoord, backgroundHeight2, 1024, 1024, 255);

			// Draw Side Wall Conjoin Lines
			CP_Settings_Stroke(darkGrey);
			CP_Graphics_DrawLine(0.0f, backgroundHeight1 + 512.0f, leftWallClosingCoord + 512.0f, backgroundHeight1 + 512.0f);
			CP_Graphics_DrawLine(0.0f, backgroundHeight2 + 512.0f, leftWallClosingCoord + 512.0f, backgroundHeight2 + 512.0f);
			CP_Graphics_DrawLine(rightWallClosingCoord - 512.0f, backgroundHeight1 + 512.0f, 1600.0f, backgroundHeight1 + 512.0f);
			CP_Graphics_DrawLine(rightWallClosingCoord - 512.0f, backgroundHeight2 + 512.0f, 1600.0f, backgroundHeight2 + 512.0f);
		
			// Draw Health Screen
			DrawDamagedView(player, maxY, damagedScreen);
		}

		#pragma endregion

	}
}

void Game_Exit(void) {

	#pragma region freeImages

	CP_Image_Free(&brickSideWalls);
	CP_Image_Free(&mainBackgroundImage);
	CP_Image_Free(&damagedScreen);
	CP_Image_Free(&playerUpgradeFalling);
	CP_Image_Free(&bulletImage);
	CP_Image_Free(&healthEmptyUI);
	CP_Image_Free(&healthFullUI);
	CP_Image_Free(&tutorial_playerControls);
	CP_Image_Free(&tutorial_playerUpgrades);
	CP_Image_Free(&tutorial_playerShooting);
	CP_Image_Free(&tutorial_flyingEnemiesWarning);
	CP_Image_Free(&tutorial_laserWarning);

	ImageUnloader(upgradeIcon, MAX_UPGRADES);
	ImageUnloader(playerCrystal, MAX_FRAME_PLAYER_CRYSTAL);
	ImageUnloader(pickupsHealth, MAX_FRAME_PICKUP_HEALTH);
	ImageUnloader(playerDefeat, MAX_FRAME_PLAYER_DEFEAT);
	ImageUnloader(playerIdle, MAX_FRAME_PLAYER_IDLE);
	ImageUnloader(playerJump, MAX_FRAME_PLAYER_JUMP);
	ImageUnloader(playerFall, MAX_FRAME_PLAYER_FALL);
	ImageUnloader(playerRight, MAX_FRAME_PLAYER_RIGHT);
	ImageUnloader(playerJumpRight, MAX_FRAME_PLAYER_JUMP_RIGHT);
	ImageUnloader(playerFallRight, MAX_FRAME_PLAYER_FALL_RIGHT);
	ImageUnloader(playerLeft, MAX_FRAME_PLAYER_LEFT);
	ImageUnloader(playerJumpLeft, MAX_FRAME_PLAYER_JUMP_LEFT);
	ImageUnloader(playerFallLeft, MAX_FRAME_PLAYER_FALL_LEFT);

	ImageUnloader(ememyWalkingLeft, MAX_FRAME_ENEMY_WALKING_LEFT);
	ImageUnloader(ememyWalkingRight, MAX_FRAME_ENEMY_WALKING_RIGHT);
	ImageUnloader(ememyFlyingLeft, MAX_FRAME_ENEMY_FLYING_LEFT);
	ImageUnloader(ememyFlyingRight, MAX_FRAME_ENEMY_FLYING_RIGHT);

	#pragma endregion

	#pragma region freeSounds

	CP_Sound_Free(&gameTrack);

	CP_Sound_Free(&player.damagedSFX);
	CP_Sound_Free(&player.jumpSFX);
	CP_Sound_Free(&player.pickupSFX);

	#pragma endregion

}
