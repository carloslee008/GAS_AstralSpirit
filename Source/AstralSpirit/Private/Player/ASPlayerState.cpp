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
	DOREPLIFETIME(AASPlayerState, XP);
	DOREPLIFETIME(AASPlayerState, AttributePoints);
	DOREPLIFETIME(AASPlayerState, SkillPoints);
}

UAbilitySystemComponent* AASPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AASPlayerState::AddToXP(int32 InXP)
{
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AASPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void AASPlayerState::AddToAttributePoints(int32 InAttributePoints)
{
	AttributePoints += InAttributePoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AASPlayerState::AddToSkillPoints(int32 InSkillPoints)
{
	SkillPoints += InSkillPoints;
	OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}

void AASPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AASPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level, false);
}

void AASPlayerState::SetAttributePoints(int32 InAttributePoints)
{
	AttributePoints = InAttributePoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AASPlayerState::SetSkillPoints(int32 InSkillPoints)
{
	SkillPoints = InSkillPoints;
	OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}

void AASPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

void AASPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void AASPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AASPlayerState::OnRep_SkillPoints(int32 OldSkillPoints)
{
	OnSkillPointsChangedDelegate.Broadcast(SkillPoints);
}


