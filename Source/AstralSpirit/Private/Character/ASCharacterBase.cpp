// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASCharacterBase.h"

#include "AbilitySystemComponent.h"

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

void AASCharacterBase::InitializePrimaryAttributes() const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(DefaultPrimaryAttributes);
	
	const FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle EffectSpecHandle= GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryAttributes, 1.f, EffectContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data, GetAbilitySystemComponent());
}
