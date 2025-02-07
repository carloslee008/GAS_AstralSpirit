// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SkillMenuWidgetController.h"

#include "ASGameplayTags.h"
#include "AbilitySystem/ASAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/ASPlayerState.h"

void USkillMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SkillPointsChanged.Broadcast(GetASPS()->GetPlayerSkillPoints());
}

void USkillMenuWidgetController::BindCallbacksToDependencies()
{
	GetASASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
	{
		if (SelectedSkill.Ability.MatchesTagExact(AbilityTag))
		{
			SelectedSkill.Status = StatusTag;
			bool bEnableSkillPoints = false;
			bool bEnableEquipped = false;
			ShouldEnableButtons(StatusTag, CurrentSkillPoints, bEnableSkillPoints, bEnableEquipped);
			FString Description;
			FString NextLevelDescription;
			GetASASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
			SkillSelectedDelegate.Broadcast(bEnableSkillPoints, bEnableEquipped, Description, NextLevelDescription);
		}
		if (AbilityInfo)
		{
			FASAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});

	GetASPS()->OnSkillPointsChangedDelegate.AddLambda([this](int32 SkillPoints)
	{
		SkillPointsChanged.Broadcast(SkillPoints);
		CurrentSkillPoints = SkillPoints;

		bool bEnableSkillPoints = false;
		bool bEnableEquipped = false;
		ShouldEnableButtons(SelectedSkill.Status, CurrentSkillPoints, bEnableSkillPoints, bEnableEquipped);
		FString Description;
		FString NextLevelDescription;
		GetASASC()->GetDescriptionsByAbilityTag(SelectedSkill.Ability, Description, NextLevelDescription);
		SkillSelectedDelegate.Broadcast(bEnableSkillPoints, bEnableEquipped, Description, NextLevelDescription);
	});
}

void USkillMenuWidgetController::SkillSelected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedSkill.Ability).AbilityType;
        StopWaitForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}

	const FASGameplayTags GameplayTags = FASGameplayTags::Get();
	const int32 SkillPoints = GetASPS()->GetPlayerSkillPoints();
	FGameplayTag AbilityStatus;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(FASGameplayTags::Get().Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetASASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;

	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetASASC()->GetStatusFromSpec(*AbilitySpec);
	}

	SelectedSkill.Ability = AbilityTag;
	SelectedSkill.Status = AbilityStatus;
	bool bEnableSkillPoints = false;
	bool bEnableEquipped = false;
	ShouldEnableButtons(AbilityStatus, SkillPoints, bEnableSkillPoints, bEnableEquipped);
	FString Description;
	FString NextLevelDescription;
	GetASASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
	SkillSelectedDelegate.Broadcast(bEnableSkillPoints, bEnableEquipped, Description, NextLevelDescription);
	
}

void USkillMenuWidgetController::SpendPointButtonPressed()
{
	if (GetASASC())
	{
		GetASASC()->ServerSpendSkillPoint(SelectedSkill.Ability);
	}
}

void USkillMenuWidgetController::SkillDeselect()
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedSkill.Ability).AbilityType;
		StopWaitForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
		
	SelectedSkill.Ability = FASGameplayTags::Get().Abilities_None;
	SelectedSkill.Status = FASGameplayTags::Get().Abilities_Status_Locked;

	SkillSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USkillMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedSkill.Ability).AbilityType;

	WaitForEquipDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;
}

void USkillMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SkillPoints,
                                                     bool& bShouldEnableSkillPointsButton, bool& bShouldEnableEquipButton)
{
	const FASGameplayTags GameplayTags = FASGameplayTags::Get();

	bShouldEnableSkillPointsButton = false;
	bShouldEnableEquipButton = false;
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (SkillPoints > 0)
		{
			bShouldEnableSkillPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		if (SkillPoints > 0)
		{
			bShouldEnableSkillPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		if (SkillPoints > 0)
		{
			bShouldEnableSkillPointsButton = true;
		}
	}
}
