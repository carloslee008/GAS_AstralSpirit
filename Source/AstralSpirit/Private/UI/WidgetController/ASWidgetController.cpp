// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/ASWidgetController.h"

#include "AbilitySystem/ASAbilitySystemComponent.h"
#include "AbilitySystem/ASAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/ASPlayerController.h"
#include "Player/ASPlayerState.h"

void UASWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UASWidgetController::BroadcastInitialValues()
{
}

void UASWidgetController::BindCallbacksToDependencies()
{
}

void UASWidgetController::BroadcastAbilityInfo()
{
	if (!GetASASC()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FASAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(GetASASC()->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = GetASASC()->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = GetASASC()->GetStatusFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	GetASASC()->ForEachAbility(BroadcastDelegate);
}

AASPlayerController* UASWidgetController::GetASPC()
{
	if (ASPlayerController == nullptr)
	{
		ASPlayerController = Cast<AASPlayerController>(PlayerController);
	}
	return ASPlayerController;
}

AASPlayerState* UASWidgetController::GetASPS()
{
	if (ASPlayerState == nullptr)
	{
		ASPlayerState = Cast<AASPlayerState>(PlayerState);
	}
	return ASPlayerState;
}

UASAbilitySystemComponent* UASWidgetController::GetASASC()
{
	if (ASAbilitySystemComponent == nullptr)
	{
		ASAbilitySystemComponent = Cast<UASAbilitySystemComponent>(AbilitySystemComponent);
	}
	return ASAbilitySystemComponent;
}

UASAttributeSet* UASWidgetController::GetASAS()
{
	if (ASAttributeSet == nullptr)
	{
		ASAttributeSet = Cast<UASAttributeSet>(AttributeSet);
	}
	return ASAttributeSet;
}
