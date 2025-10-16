#pragma once
//------------------------------------------------------------------------------------
// file: enemy.h
//
// authors:
// tituswenshuen.kwong (primary author)
//		Added ContinuousSpawningFlying()
//		Added UpdateFlyingEnemies()
//		Added BulletHitFlyingEnemy()
//		Added FlyingEnemyTouchPlayer()
//		Added DespawnFlyingEnemy()
// l.kangweimatthew 
//		Added ContinuousSpawningWalking()
//		Added WalkingEnemyLogic()
//		Added BulletHitWalkingEnemy()
//		Added WalkingEnemyTouchPlayer()
//		DespawnWalkingEnemy()
//
// brief:
// Contains the function definitions for spawning flying and walking enemies and their respective behaviour logic 
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

/// <summary>
/// Spawn Flying Enemies outside of Screen, below Player.
/// </summary>
/// <param name="enemies">Array of Flying Enemies</param>
/// <param name="enemyCount">Number of Flying Enemies to Spawn</param>
/// <param name="maxY">Midpoint of Screen Y Value</param>
/// <param name="healthModifier">Increment Health Modifier for Spawned Enemy. Leave as 0 to spawn base health</param>
/// <param name="speedModifier">Increment Speed Modifier for Spawned Enemy. Leave as 0 to spawn base speed</param>
void ContinuousSpawningFlying(struct Flying_Enemy enemies[], int enemyCount, float maxY, int healthModifier, int speedModifier);

/// <summary>
/// Handles Flying of Active Flying Enemies.
/// </summary>
/// <param name="player">Player</param>
/// <param name="enemies">Array of Flying Enemies</param>
void UpdateFlyingEnemies(struct Player player, struct Flying_Enemy enemies[]);

/// <summary>
/// Handles Bullet Enemy Interaction.
/// </summary>
/// <param name="bullets">Array of Bullet</param>
/// <param name="enemies">Array of Flying Enemies</param>
/// <param name="player">Player</param>
void BulletHitFlyingEnemy(struct Bullet bullets[], struct Flying_Enemy enemies[], struct Player* player);

/// <summary>
/// Handles Enemy Player Interaction.
/// </summary>
/// <param name="enemies">Array of Flying Enemies</param>
/// <param name="player">Address of Player</param>
void FlyingEnemyTouchPlayer(struct Flying_Enemy enemies[], struct Player* player);

/// <summary>
/// Deletes Flying Enemies that can't catch up to Player.
/// </summary>
/// <param name="enem">Array of Flying Enemies</param>
/// <param name="maxY">Midpoint of Screen Y</param>
void DespawnFlyingEnemy(struct Flying_Enemy enem[], float maxY);

/// <summary>
/// Spawns Walking Enemies on Platforms.
/// </summary>
/// <param name="enem">Array of Walking Enemies</param>
/// <param name="enemyCount">Number of Enemies to Spawn</param>
/// <param name="platforms">Array of Platforms</param>
/// <param name="healthModifier">Increment Health Modifier for Spawned Enemy. Leave as 0 to spawn base health</param>
/// <param name="speedModifier">Increment Speed Modifier for Spawned Enemy. Leave as 0 to spawn base speed</param>
void ContinuousSpawningWalking(struct Walking_Enemy enem[], int enemyCount, struct Platform platforms[], int healthModifier, int speedModifier);

/// <summary>
/// Handles the walking of Active walking Enemies.
/// </summary>
/// <param name="player">Player</param>
/// <param name="enem">Array of Walking Enemies</param>
void WalkingEnemyLogic(struct Player player, struct Walking_Enemy enem[]);

/// <summary>
/// Handles Bullet Enemy Interaction.
/// </summary>
/// <param name="bullets">Array of Bullet</param>
/// <param name="enemies">Array of Walking Enemies</param>
/// <param name="player">Player</param>
/// <param name="pickup">Array of Health Pickups</param>
void BulletHitWalkingEnemy(struct Bullet bullets[], struct Walking_Enemy enemies[], struct Player* player, struct Pickups pickup[]);

/// <summary>
/// Handles Enemy Player Interaction.
/// </summary>
/// <param name="enemies">Array of Walking Enemies</param>
/// <param name="player">Address of Player</param>
void WalkingEnemyTouchPlayer(struct Walking_Enemy enemies[], struct Player* player);

/// <summary>
/// Deletes Walking Enemies that leave the Screen.
/// </summary>
/// <param name="enem">Array of Walking Enemies</param>
/// <param name="maxY">Midpoint of Screen Y</param>
void DespawnWalkingEnemy(struct Walking_Enemy enem[], float maxY);
