#pragma once
//------------------------------------------------------------------------------------
// file: upgrades.h
//
// authors:
// l.zheneudamon (primary author)
//		Added declarations
//
// brief:
// Contains declaration of functions used in upgrades.c.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

/// <summary>
/// Updates an Array with random available upgrades.
/// </summary>
/// <param name="chosenUpgrades">Array of size 3. This will contain the output</param>
/// <param name="playerUpgrades">Array of upgrades that the player already has</param>
void Get3AvailableUpgrades(int chosenUpgrades[], int playerUpgrades[]);

/// <summary>
/// Detects which upgrade is being selected.
/// </summary>
/// <param name="maxY">Middle of screen Y value</param>
/// <returns>Integer of 1,2,3 for upgrades, or 0 if none selected</returns>
int GetUpgradeSelected(float maxY);
