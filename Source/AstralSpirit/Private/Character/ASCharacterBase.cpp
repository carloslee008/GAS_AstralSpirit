// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "ASGameplayTags.h"
#include "AbilitySystem/ASAbilitySystemComponent.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "AstralSpirit/AstralSpirit.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

AASCharacterBase::AASCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	IgniteDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("IgniteDebuffComponent");
	IgniteDebuffComponent->SetupAttachment(GetRootComponent());
	IgniteDebuffComponent->DebuffTag = FASGameplayTags::Get().Debuff_Ignite;

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
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
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
	OnDeath.Broadcast(this);
}

void AASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

FVector AASCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	const FASGameplayTags GameplayTags = FASGameplayTags::Get();
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Tail))
	{
		return GetMesh()->GetSocketLocation(TailSocketName);
	}
	return FVector();
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

UNiagaraSystem* AASCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FTaggedMontage AASCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag == MontageTag)
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

int32 AASCharacterBase::GetMinionCount_Implementation()
{
	return MinionCount;
}

void AASCharacterBase::UpdateMinionCount_Implementation(int32 Amount)
{
	MinionCount += Amount;
}

ECharacterClass AASCharacterBase::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

FOnASCRegistered AASCharacterBase::GetOnASCRegisteredDelegate()
{
	return OnAscRegistered;
}

FOnDeath* AASCharacterBase::GetOnDeathDelegate()
{
	return &OnDeath;
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
	ASASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
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
