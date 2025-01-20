// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(float XP)
{
	
	int32 MaxLevel = LevelUpInformation.Num() - 1;
	// In case mount of XP greater than max level requirement
	if (XP >= LevelUpInformation[MaxLevel].LevelUpRequirement) return MaxLevel; 
	
	// Start at index 1 because entry for level "0" is a placeholder
	// LevelUpInformation[1] = Level 1 Info
	// LevelUpInformation[2] = Level 2 Info
	for (int32 Level = 1; Level < LevelUpInformation.Num(); Level++)
	{
		if (XP < LevelUpInformation[Level].LevelUpRequirement)
		{
			return Level;
		}
	}
	return 1;
}
