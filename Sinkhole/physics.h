#pragma once
//------------------------------------------------------------------------------------
// file: physics.h
//
// authors:
// l.kangweimatthew (primary author)
//		Added IsRightWallCollided()
//		Added IsLeftWallCollided()
//		Added IsCeilingCollided()
// l.zheneudamon 
//		Added IsPlayerStopped()
//
// brief:
// Contains the physics functions headers that are used in the game to detect collision.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

/// <summary>
/// IsRightWallCollided() checks for collision happening with the right wall.
/// </summary>
/// <param name="playerX"> X coordinate of player </param>
/// <param name="playerWidth"> Width of the player </param>
/// <param name="wallWidth"> Width of the Wall </param>
/// <returns> 1 (collided) or 0 (not collided) </returns>
int IsRightWallCollided(float playerX, float playerWidth, float wallWidth);

/// <summary>
/// IsLeftWallCollided() checks for collision happening with the left wall.
/// </summary>
/// <param name="playerX"> X Coordinate of Pslayer </param>
/// <param name="playerWidth"> Width of the Player </param>
/// <param name="wallWidth"> Width of the Wall </param>
/// <returns> 1 (collided) or 0 (not collided) </returns>
int IsLeftWallCollided(float playerX, float playerWidth, float wallWidth);

/// <summary>
/// IsCeilingCollided() checks for collision happening with the ceiling.
/// </summary>
/// <param name="playerY"> Y Coordinate of Player </param>
/// <param name="playerHeight"> Height of the Player </param>
/// <param name="ceilingHeight"> Width of the Wall </param>
/// <returns> 1 (collided) or 0 (not collided) </returns>
int IsCeilingCollided(float playerY, float playerHeight, float ceilingHeight);

/// <summary>
/// Determines if player is halted horizontally or vertically.
/// ONLY CALL THIS FUNCTION WHEN player's next frame is inside a platform.
/// </summary>
/// <param name="player"> Player </param>
/// <param name="scenario">
/// 1 if player is moving bottom right, platform corner is top left.
/// 2 if player is moving bottom left, platform corner is top right.
/// 3 if player is moving top right, platform corner is bottom left.
/// 4 if player is moving top left, platform corner is bottom right.
/// </param>
/// <param name="platfromCornerX"> X Coordinate of Platform Corner, based on Scenario </param>
/// <param name="platformCornerY"> Y Coordinate of Platform Corner, based on Scenario </param>
/// <returns> 1 if faceplanted, else 0 </returns>
int IsPlayerStopped(struct Player player, int scenario, float platfromCornerX, float platformCornerY);
