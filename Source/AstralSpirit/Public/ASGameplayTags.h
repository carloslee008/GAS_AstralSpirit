// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * ASGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */


struct FASGameplayTags
{
public:
    static const FASGameplayTags& Get() { return GameplayTags; }
    static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Dexterity;
	
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_DodgeChance;
	FGameplayTag Attributes_Secondary_FireResistance;
	FGameplayTag Attributes_Secondary_ColdResistance;
	FGameplayTag Attributes_Secondary_LightningResistance;
	FGameplayTag Attributes_Secondary_PoisonResistance;
	
protected:

private:
    static FASGameplayTags GameplayTags;
};
