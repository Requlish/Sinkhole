#pragma once
//------------------------------------------------------------------------------------
// file: pickups.h
//
// authors:
// l.zheneudamon (primary author)
//		Added declarations
//
// brief:
// Contains declaration of functions used in pickups.c.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

/// <summary>
/// Spawns a Health Pickup.
/// </summary>
/// <param name="pickups">Array of Pickups</param>
/// <param name="pickupX">Spawning X Coordinate</param>
/// <param name="pickupY">Spawning Y Coordinate</param>
/// <param name="pickupSize">Size of Pickup</param>
/// <param name="percent">Number from 0 - 100 to determine chance of spawning</param>
void SpawnHealthPickup(struct Pickups pickups[], float pickupX, float pickupY, int percent);

/// <summary>
/// Checks if the Player has picked up a Pickup.
/// </summary>
/// <param name="player">Player</param>
/// <param name="pickups">Array of Pickups</param>
/// <returns>1 if the Player picked up a Pickup</returns>
int CheckPlayerPickedUpHealth(struct Player player, struct Pickups pickups[]);

/// <summary>
/// Sets Pickups to 0.
/// </summary>
/// <param name="pickups">Array of Pickups</param>
/// <param name="maxY">Midpoint of the screen Y</param>
void DespawnHealthPickup(struct Pickups pickups[], float maxY);

/// <summary>
/// Changes the address of pickup images that are active.
/// </summary>
/// <param name="pickups">Array of Pickups</param>
/// <param name="gameTimer">Timer that increases per frame</param>
void AnimatePickupHealths(struct Pickups pickups[], CP_Image pickupsHealth[], float gameTimer);
