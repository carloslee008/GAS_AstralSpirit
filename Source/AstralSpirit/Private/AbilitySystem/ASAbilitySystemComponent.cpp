// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ASAbilitySystemComponent.h"

void UASAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UASAbilitySystemComponent::EffectApplied);
}

void UASAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                              const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Blue, FString("Effect Applied!"));
}
