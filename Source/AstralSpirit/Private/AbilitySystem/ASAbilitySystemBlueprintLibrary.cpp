// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ASAbilitySystemBlueprintLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Player/ASPlayerState.h"
#include "UI/HUD/ASHUD.h"
#include "UI/WidgetController/ASWidgetController.h"

UOverlayWidgetController* UASAbilitySystemBlueprintLibrary::GetOverlayWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AASHUD* ASHUD = Cast<AASHUD>(PC->GetHUD()))
		{
			AASPlayerState* PS = PC->GetPlayerState<AASPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return ASHUD->GetOverlayWidgetController(WidgetControllerParams);
		}	
	}
	return nullptr;
}

UAttributeMenuWidgetController* UASAbilitySystemBlueprintLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AASHUD* ASHUD = Cast<AASHUD>(PC->GetHUD()))
		{
			AASPlayerState* PS = PC->GetPlayerState<AASPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return ASHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}
