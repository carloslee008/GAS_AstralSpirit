// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ASPlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/ASAbilitySystemComponent.h"
#include "AbilitySystem/ASAttributeSet.h"
#include "Net/UnrealNetwork.h"

AASPlayerState::AASPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UASAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UASAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.f;
}

void AASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AASPlayerState, Level);
}

UAbilitySystemComponent* AASPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AASPlayerState::OnRep_Level(int32 OldLevel)
{
}
