#pragma once
//------------------------------------------------------------------------------------
// file: difficulty.h
//
// authors:
// l.kangweimatthew (primary author)
//		Added DifficultyPlatformModifier()
//		Added DifficultyModifier()
// l.zheneudamon 
//		Assisted DifficultyPlatformModifier()
//		Assisted DifficultyModifier()
//
// brief:
// Contains the function headers that control the difficulty scaling and logic of the game
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

/// <summary>
/// Spawns Enemies on Platform based on player's Progression.
/// </summary>
/// <param name="playerTotalUpgrades"> Total Number of Player Upgardes </param>
/// <param name="walkingEnemies"> Array of Walking Enemies </param>
/// <param name="platforms"> Array of Platforms </param>
void DifficultyPlatformModifier(int playerTotalUpgrades, struct Walking_Enemy walkingEnemies[], struct Platform platforms[]);

/// <summary>
/// Spawns Flying Enemies and Hazards based on player's Progression.
/// </summary>
/// <param name="playerTotalUpgrades"> Total Number of Player Upgardes </param>
/// <param name="flyingEnemies"> Array of Flying Enemies </param>
/// <param name="maxY"> Max Y Value </param>
/// <param name="gameTimer"> Game Timer </param>
/// <param name="lastSpawnedTime"> Time since last Spawn </param>
void DifficultyModifier(int playerTotalUpgrades, struct Flying_Enemy flyingEnemies[], struct Lazer_Hazard lazerHazard[], float maxY, float gameTimer, float* lastSpawnedTime, float* lazerSpawnedTimer, int* nextLazerTimer);
