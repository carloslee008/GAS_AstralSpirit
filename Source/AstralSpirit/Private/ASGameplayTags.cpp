// Fill out your copyright notice in the Description page of Project Settings.


#include "ASGameplayTags.h"
#include "GameplayTagsManager.h"

FASGameplayTags FASGameplayTags::GameplayTags;

void FASGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces physical damage taken"));
}
