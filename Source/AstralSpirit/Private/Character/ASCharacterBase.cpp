// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/ASAbilitySystemComponent.h"
#include "AstralSpirit/AstralSpirit.h"
#include "Components/CapsuleComponent.h"

AASCharacterBase::AASCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

UAbilitySystemComponent* AASCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* AASCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AASCharacterBase::Die()
{
	// Detach the weapon from character. This is done on Server only
	if (Weapon)
	{
		Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	}
	MulticastHandleDeath();
}

void AASCharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	// Ragdoll the character
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	// Prevent blocking other characters
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); 

	Dissolve();
	bDead = true;
}

void AASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

FVector AASCharacterBase::GetCombatSocketLocation_Implementation()
{
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

bool AASCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AASCharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> AASCharacterBase::GetTaggedMontages_Implementation()
{
	return AttackMontages;
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

void AASCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* MaterialInstanceDynamic = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, MaterialInstanceDynamic);
		StartDissolveTimeline(MaterialInstanceDynamic);
	}
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* MaterialInstanceDynamic = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, MaterialInstanceDynamic);
		StartWeaponDissolveTimeline(MaterialInstanceDynamic);
	}
}
