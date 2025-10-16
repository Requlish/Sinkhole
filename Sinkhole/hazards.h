#pragma once
//------------------------------------------------------------------------------------
// file: hazards.h
//
// authors:
// l.zheneudamon (primary author)
//		Added declarations
//
// brief:
// Contains declaration of functions used in hazards.c.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

/// <summary>
/// Spawns a Lazer Hazard.
/// </summary>
/// <param name="lazerHazard">Array of Lazer Hazards</param>
/// <param name="timeToFire">Time it takes for Lazer To Fire</param>
/// <param name="y">Center of Lazer's Y Value</param>
/// <param name="height">Height of Lazer</param>
/// <param name="gameTime">Current Game Time</param>
void SpawnLazerHazard(struct Lazer_Hazard lazerHazard[], float timeToFire, float y, float height, float gameTime);

/// <summary>
/// Checks if the Lazer hit the player.
/// </summary>
/// <param name="player">Address of Player</param>
/// <param name="lazerHazard">Address of Expired Lazer Hazard</param>
/// <returns>1 if lazer hits, 0 if lazer didn't hit</returns>
int DidLazerHitPlayer(struct Player* player, struct Lazer_Hazard* lazerHazard);

/// <summary>
/// Removes Lazers that are outside of the Screen.
/// </summary>
/// <param name="lazerHazard">Array of Lazer Hazards</param>
/// <param name="maxY">Center Point of Camera</param>
void DespawnLazerHazard(struct Lazer_Hazard lazerHazard[], float maxY);
