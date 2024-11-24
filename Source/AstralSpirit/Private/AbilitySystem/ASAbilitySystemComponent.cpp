// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ASAbilitySystemComponent.h"

#include "ASGameplayTags.h"

void UASAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UASAbilitySystemComponent::EffectApplied);

	const FASGameplayTags& GameplayTags = FASGameplayTags::Get();
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Yellow,
		FString::Printf(TEXT("Tag: %s"), *GameplayTags.Attributes_Secondary_Armor.ToString())
	);
}

void UASAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                              const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
	
}
