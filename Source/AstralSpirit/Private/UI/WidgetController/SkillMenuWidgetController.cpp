// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SkillMenuWidgetController.h"

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
	GetASASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
	{
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
	});
}
