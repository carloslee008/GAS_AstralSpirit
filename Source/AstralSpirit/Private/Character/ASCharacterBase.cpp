// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/ASAbilitySystemComponent.h"

AASCharacterBase::AASCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

UAbilitySystemComponent* AASCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AASCharacterBase::InitAbilityActorInfo()
{
}

void AASCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> GameplayEffect, const float ActorLevel) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffect);

	const FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle EffectSpecHandle= GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffect, ActorLevel, EffectContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data, GetAbilitySystemComponent());
}

void AASCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void AASCharacterBase::AddCharacterAbilities()
{
	UASAbilitySystemComponent* ASASC = CastChecked<UASAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	ASASC->AddCharacterAbilities(StartupAbilities);

	
}
