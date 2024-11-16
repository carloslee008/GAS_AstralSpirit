// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ASPlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/ASAbilitySystemComponent.h"
#include "AbilitySystem/ASAttributeSet.h"

AASPlayerState::AASPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UASAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UASAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* AASPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
