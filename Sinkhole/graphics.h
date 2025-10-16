#pragma once
//------------------------------------------------------------------------------------
// file: graphics.h
//
// authors:
// l.zheneudamon (primary author)
//		Added Health UI
//		Added Upgrade related functions
//		Added Health Pickups
//		Added Lazer Hazards
//		Added Damaged View
// timo.duethorn
//		Added drawing of Platforms
//		Added drawing of Pause Menu
//		Added drawing of Defeat Text
// tituswenshuen.kwong
//		Flying Enemy drawing
//		Bullet Drawing
// l.kangweimatthew
//		Walking Enemy drawing
//
// brief:
// Contains declaration of functions used in graphics.c.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

/// <summary>
/// Draws the UI for Player Health Bar.
/// </summary>
/// <param name="healthEmptyUI"> Image of empty heart </param>
/// <param name="healthFullUI"> Image of full heart </param>
/// <param name="playerCurrentHealth"> Player's current health </param>
/// <param name="playerMaxHealth"> Player's max health </param>
/// <param name="imageSize"> Heart Size </param>
/// <param name="startingYHeight"> Starting Y Value of UI </param>
/// <param name="wallWidth"> Side Wall Width </param>
void DrawHeatlhUI(CP_Image healthEmptyUI, CP_Image healthFullUI, int playerCurrentHealth, int playerMaxHealth, float imageSize, float startingYHeight, float wallWidth);

/// <summary>
/// Draws the Platforms.
/// </summary>
/// <param name="platforms"> Array of Platforms </param>
/// <param name="drawnPlatforms"> Number of drawn Platforms </param>
/// <param name="platformHeight"> The actual pixel height of each individual Platform </param>
void DrawPlatforms(struct Platform platforms[], int drawnPlatforms, float platformHeight);

/// <summary>
/// Draws the upgrade selection with animations.
/// </summary>
/// <param name="upgradeIcon1"> Icon of Upgrade Choice 1 </param>
/// <param name="upgradeName1"> Name of Upgrade Choice 1 </param>
/// <param name="upgradeText1"> Text of Upgrade Choice 1 </param>
/// <param name="upgradeIcon2"> Icon of Upgrade Choice 2 </param>
/// <param name="upgradeName2"> Name of Upgrade Choice 2 </param>
/// <param name="upgradeText2"> Text of Upgrade Choice 2 </param>
/// <param name="upgradeIcon3"> Icon of Upgrade Choice 3 </param>
/// <param name="upgradeName3"> Name of Upgrade Choice 3 </param>
/// <param name="upgradeText3"> Text of Upgrade Choice 3 </param>
/// <param name="upgradeFont"> Font of Upgrade Text </param>
/// <param name="maxY"> Midpoint of Screen Y </param>
/// <param name="animationTime"> Time that increment based on delta time </param>
/// <param name="fadeTime"> Duration of the animation </param>
/// <param name="selectedUpgrade"> Upgrade that is selected </param>
void DrawUpgradeSelection(CP_Image upgradeIcon1, char upgradeName1[], char upgradeText1[], CP_Image upgradeIcon2, char upgradeName2[], char upgradeText2[], CP_Image upgradeIcon3, char upgradeName3[], char upgradeText3[], CP_Font upgradeFont, float maxY, float animationTime, float fadeTime, int selectedUpgrade);

/// <summary>
/// Draws the fade out upgrade selection screen.
/// </summary>
/// <param name="upgradeIcon"> Selected Upgrade Icon </param>
/// <param name="upgradeName"> Selected Upgrade Name </param>
/// <param name="upgradeText"> Selected Upgrade Text </param>
/// <param name="upgradeFont"> Font of Upgrade Text </param>
/// <param name="maxY"> Midpoint of Screen Y </param>
/// <param name="animationTime"> Time that increment based on delta time </param>
/// <param name="fadeTime"> Duration of the animation </param>
/// <param name="selectedUpgrade"> Upgrade that was selected </param>
void DrawAfterUpgradeSelection(CP_Image upgradeIcon, char upgradeName[], char upgradeText[], CP_Font upgradeFont, float maxY, float animationTime, float fadeTime, int selectedUpgrade);

/// <summary>
/// Draws the UI for Upgrades.
/// </summary>
/// <param name="upgradeIcon"> Array of Upgrade Icons </param>
/// <param name="playerUpgrades"> Array of Player's Upgrades </param>
/// <param name="imageSize"> Upgrade Icon Size </param>
/// <param name="startingYHeight"> Starting Y Value of UI </param>
/// <param name="wallWidth"> Side Wall Width </param>
void DrawUpgradeUI(CP_Image upgradeIcon[], int playerUpgrades[], float imageSize, float startingYHeight, float wallWidth);

/// <summary>
/// Draw the Pause Menu UI.
/// </summary>
/// <param name="maxY"> Midpoint of Screen Y </param>
/// <param name="selectedButton"> Button that is Hovered </param>
void DrawPauseMenu(float maxY, int selectedButton);

/// <summary>
/// Draw the text for defeat UI.
/// </summary>
/// <param name="maxY"> Midpoint of Screen Y </param>
/// <param name="selectedButton"> Button that is Hovered </param>
void DrawDefeatText(float maxY, float scoreMultiplier, int selectedButton);

/// <summary>
/// Draws the Pickups.
/// </summary>
/// <param name="pickup"> Array of Pickups </param>
void DrawPickupHealth(struct Pickups pickup[]);

/// <summary>
/// Draws the Bullets.
/// </summary>
/// <param name="bullets"> Array of Bullets </param>
/// <param name="bulletColor"> Color of Bullet </param>
void DrawBullets(struct Bullet bullets[], CP_Image bulletImage);

/// <summary>
/// Draws flying Enemies.
/// </summary>
/// <param name="enemies"> Flying Enemy Struct </param>
/// <param name="player"> Player </param>
/// <param name="enemyLeft"> Array of Frames of Enemy Flying Left </param>
/// <param name="enemyRight"> Array of Frames of Enemy Flying Right </param>
/// <param name="gameTimer"> Time since game started </param>
void DrawFlyingEnemies(struct Flying_Enemy enemies[], struct Player player, CP_Image enemyLeft[], CP_Image enemyRight[], float gameTimer);

/// <summary>
/// Draws Lazer Hazard.
/// </summary>
/// <param name="lazerHazard"> Array of Lazer Hazards </param>
/// <param name="gameTimer"> gameTimer </param>
/// <param name="wallWidth"> Side Wall's Width </param>
void DrawLazerHazard(struct Lazer_Hazard lazerHazard[], float gameTimer, float wallWidth);

/// <summary>
/// Draws Walking Enemies.
/// </summary>
/// <param name="enemies"> Walking Enemy Struct </param>
/// <param name="enemyLeft"> Array of Frames of Enemy Walking Left </param>
/// <param name="enemyRight"> Array of Frames of Enemy Walking Right </param>
/// <param name="gameTimer"> Time since game started </param>
void DrawWalkingEnemies(struct Walking_Enemy enemies[], CP_Image enemyLeft[], CP_Image enemyRight[], float gameTimer);

/// <summary>
/// Draws the Damaged Health View.
/// </summary>
/// <param name="player"> Player </param>
/// <param name="maxY"> Midpoint of Screen Y </param>
/// <param name="damagedScreen"> Translucent Shades of Red </param>
void DrawDamagedView(struct Player player, float maxY, CP_Image damagedScreen);
