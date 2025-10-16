#pragma once
//------------------------------------------------------------------------------------
// file: projectiles.h
//
// authors:
// tituswenshuen.kwong
//		Added bullet shooting functions
//
// brief:
// Contains the function headers that deal with the shooting logic of bullets
// FireBullet - player bullet firing
// UpdateBullet - bullet updating after hitting enemy, and side walls
// BulletHitPlatform - bullet updating after hitting platforms
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

/// <summary>
/// Attempts to fire a bullet. Do this on left clicks.
/// </summary>
/// <param name="bullets">Array of Bullets</param>
/// <param name="player">Player</param>
/// <param name="mouseX">Mouse World X Value</param>
/// <param name="mouseY">Mouse World Y Value</param>
void FireBullet(struct Bullet bullets[], struct Player player, float mouseX, float mouseY);

/// <summary>
/// Moves and Checks if bullets hit anything
/// </summary>
/// <param name="bullets">Array of Bullets</param>
/// <param name="maxY">Midpoint of Screen Y</param>
/// <param name="wallWidth">Width of Side Walls</param>
void UpdateBullets(struct Bullet bullets[], float maxY, float wallWidth);

/// <summary>
/// Handles the logic of a bullet hitting a platform.
/// </summary>
/// <param name="bullets">Array of Bullets</param>
/// <param name="platforms">Array of Platforms</param>
/// <param name="platformHeight">Platform Height</param>
void BulletHitPlatform(struct Bullet bullets[], struct Platform platforms[], float platformHeight);
