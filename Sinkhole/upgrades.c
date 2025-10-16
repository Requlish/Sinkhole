//------------------------------------------------------------------------------------
// file: upgrades.c
//
// authors:
// l.zheneudamon (primary author)
//		Added all upgrades functions
//
// brief:
// Contains functions used for upgrades.
//
// documentation link:
// https://github.com/gwong-dp/c_processing/wiki
//
// All Content © 2023 DigiPen Institute of Technology Singapore, All rights reserved.
//------------------------------------------------------------------------------------

#include <math.h>
#include "cprocessing.h"
#include "initialize.h"
#include "utils.h"

void Get3AvailableUpgrades(int chosenUpgrades[], int playerUpgrades[]) {
	// Create an Array to hold available options to RNG from
	// Since upgrades 1-3 are always available, they are here first
	int availableUpgrades[MAX_UPGRADES] = { 1,2,3 };

	// By the End of this Loop, Limited Upgrades that are not taken
	// will populate the availableUpgrades array (will be empty spaces)
	for (int i = 3; i < MAX_UPGRADES; i++) {
		if (playerUpgrades[i] == 0) {
			availableUpgrades[i] = i + 1; // +1 so upgrade 1 (in element 0) is now 1
		}
	}

	// This section calculates the max number of upgrades (maxElements) to RNG from
	// by counting the number of 0s in availableUpgrades
	// It also shifts the non-zeros to the front
	int numberOfZeros = 0;
	for (int i = 0; i < MAX_UPGRADES; i++) {
		if (availableUpgrades[i] == 0){
			numberOfZeros++;
		} else {
			availableUpgrades[i-numberOfZeros] = availableUpgrades[i];
		}
	}

	int maxElements = MAX_UPGRADES - numberOfZeros;
	for (int i = 0; i + maxElements < MAX_UPGRADES; i++) {
		availableUpgrades[i + maxElements] = 0;
	}

	// This final part RNGs a number and adds the equivalent upgrade as a option
	// The rest of this of this code checks that the previously chosen upgrade
	// is not already chosen again.
	int count = 0;
	int rngedUpgrade = 0;
	int chosenUpgrade1 = 0;
	int chosenUpgrade2 = 0;
	int chosenUpgrade3 = 0;
	while (count < 3) {
		int randInt = RandomNumber(0, maxElements-1);
		rngedUpgrade = availableUpgrades[randInt];
		if (chosenUpgrade1) {
			if (chosenUpgrade1 != rngedUpgrade) {
				if (chosenUpgrade2) {
					if (chosenUpgrade2 != rngedUpgrade) {
						chosenUpgrade3 = rngedUpgrade;
						count++;
					}
				} else {
					chosenUpgrade2 = rngedUpgrade;
					count++;
				}
			}
		} else {
			chosenUpgrade1 = rngedUpgrade;
			count++;
		}
	}
	chosenUpgrades[0] = chosenUpgrade1 - 1; // -1 to Represent Element of Upgrade
	chosenUpgrades[1] = chosenUpgrade2 - 1; 
	chosenUpgrades[2] = chosenUpgrade3 - 1; 
}

int GetUpgradeSelected(float maxY) {
	float upgrade1X = CP_System_GetWindowWidth() * 0.25f;
	float upgrade2X = CP_System_GetWindowWidth() * 0.50f;
	float upgrade3X = CP_System_GetWindowWidth() * 0.75f;
	float width = CP_System_GetWindowWidth() * 0.2f;
	float height = CP_System_GetWindowHeight() * 0.8f;

	float screenShift = maxY - CP_System_GetWindowHeight() / 2.0f;
	if (IsAreaClicked(upgrade1X, maxY, width, height, CP_Input_GetMouseX(), CP_Input_GetMouseY() + screenShift)) {
		return 1;
	} else if (IsAreaClicked(upgrade2X, maxY, width, height, CP_Input_GetMouseX(), CP_Input_GetMouseY() + screenShift)) {
		return 2;
	} else if (IsAreaClicked(upgrade3X, maxY, width, height, CP_Input_GetMouseX(), CP_Input_GetMouseY() + screenShift)) {
		return 3;
	} else{
		return 0;
	}	
}
