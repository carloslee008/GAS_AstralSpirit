// Fill out your copyright notice in the Description page of Project Settings.


#include "ASGameplayTags.h"
#include "GameplayTagsManager.h"

FASGameplayTags FASGameplayTags::GameplayTags;

void FASGameplayTags::InitializeNativeGameplayTags()
{
	/*
	 * Primary Attributes
	 */
	 
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increases Armor, Health Regeneration, and Block Chance"));
	
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Primary.Intelligence"),
	 FString("Increases Mana, Mana Regeneration, and Elemental Resistances"));
	
	GameplayTags.Attributes_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Primary.Dexterity"),
	 FString("Increases Critical Hit Chance, Critical Hit Damage, and Dodge Chance"));

	/*
	 * Secondary Attributes
	 */

	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.MaxHealth"),
	 FString("Max Health"));
	
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.MaxMana"),
	 FString("Max Mana"));

	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.Armor"),
	 FString("Reduces physical damage taken"));
	
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.HealthRegeneration"),
	 FString("Amount of health regenerated per 1 second"));
	
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.ManaRegeneration"),
	 FString("Amount of mana regenerated per 1 second"));
	
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.CriticalHitChance"),
	 FString("Chance of dealing critical damage"));
	
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.CriticalHitDamage"),
	 FString("Amount of damage dealt when dealing critical hits"));
	
	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.BlockChance"),
	 FString("Chance to block incoming damage"));
	
	GameplayTags.Attributes_Secondary_DodgeChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.DodgeChance"),
	 FString("Chance of avoiding damage"));
	
	GameplayTags.Attributes_Secondary_FireResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.FireResistance"),
	 FString("Reduces fire damage taken"));
	
	GameplayTags.Attributes_Secondary_ColdResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.ColdResistance"),
	 FString("Reduces cold damage taken"));
	
	GameplayTags.Attributes_Secondary_LightningResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.LightningResistance"),
	 FString("Reduces lightning damage taken"));
	
	GameplayTags.Attributes_Secondary_PoisonResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.PoisonResistance"),
	 FString("Reduces poison damage taken"));

}
