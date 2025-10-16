#pragma once
//------------------------------------------------------------------------------------
// file: defeat.h
//
// authors:
// l.zheneudamon (primary author)
//		Added declarations
//
// brief:
// Contains declaration of functions used in defeat.c.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

/// <summary>
/// Determines which button is hovered.
/// </summary>
/// <param name="maxY"> Midpoint of Screen Y </param>
/// <returns> 1 for Respawn, 2 for Main Menu, & 0 for nothing hovered </returns>
int GetDefeatButtonSelected(float maxY);
