#pragma once
//------------------------------------------------------------------------------------
// file: mainmenu.h
//
// authors:
// timo.duethorn (primary author)
//		Added declarations
//
// brief:
// Contains the declaration of functions in platforms.c.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

/// <summary>
/// Randomly generates platform data at last value of platforms array.
/// </summary>
/// <param name="platforms"> Array of Platforms </param>
/// <param name="i"> Iterator </param>
/// <param name="wallWidth"> Side Wall Width </param>
void GeneratePlatform(struct Player player, struct Platform platforms[], int i, float wallWidth);

/// <summary>
///	Shifts platforms array forward by one, deleting first entry and generating a new platform in last entry.
/// </summary>
/// /// <param name="platforms"> Array of Platforms </param>
/// /// <param name="drawnPlatforms"> Number of drawn Platforms </param>
/// /// <param name="newY"> Y value of newly generated Platform </param>
void ShiftPlatform(struct Player player, struct Platform platforms[], int drawnPlatforms, int newY, float wallWidth);
