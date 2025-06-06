// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ASCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "ASGameplayTags.h"
#include "AbilitySystem/ASAbilitySystemComponent.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"
#include "AstralSpirit/AstralSpirit.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AASCharacterBase::AASCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	IgniteDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("IgniteDebuffComponent");
	IgniteDebuffComponent->SetupAttachment(GetRootComponent());
	IgniteDebuffComponent->DebuffTag = FASGameplayTags::Get().Debuff_Ignite;

	StunDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("StunDebuffComponent");
	StunDebuffComponent->SetupAttachment(GetRootComponent());
	StunDebuffComponent->DebuffTag = FASGameplayTags::Get().Debuff_Stun;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	EffectAttachComponent = CreateDefaultSubobject<USceneComponent>("EffectAttachPoint");
	EffectAttachComponent->SetupAttachment(GetRootComponent());
	EffectAttachComponent->SetUsingAbsoluteRotation(true);
	EffectAttachComponent->SetWorldRotation(FRotator::ZeroRotator);
	AstralProtectionNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("AstralProtectionComponent");
	AstralProtectionNiagaraComponent->SetupAttachment(EffectAttachComponent);
	LifeSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("LifeSiphonNiagaraComponent");
	LifeSiphonNiagaraComponent->SetupAttachment(EffectAttachComponent);
	ManaSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("ManaSiphonNiagaraComponent");
	ManaSiphonNiagaraComponent->SetupAttachment(EffectAttachComponent);

}

void AASCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AASCharacterBase, bIsStunned);
	DOREPLIFETIME(AASCharacterBase, bIsIgnited);
}

float AASCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	const float DamageTaken = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	OnDamageDelegate.Broadcast(DamageTaken);
	return DamageTaken;
}

UAbilitySystemComponent* AASCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* AASCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AASCharacterBase::Die(const FVector& DeathImpulse)
{
	// Detach the weapon from character. This is done on Server only
	if (Weapon)
	{
		Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	}
	MulticastHandleDeath(DeathImpulse);
}

void AASCharacterBase::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Weapon->AddImpulse(DeathImpulse * 0.1f, NAME_None, true);

	// Ragdoll the character
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->AddImpulse(DeathImpulse, NAME_None, true);
	
	// Prevent blocking other characters
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); 

	Dissolve();
	bDead = true;
	IgniteDebuffComponent->Deactivate();
	StunDebuffComponent->Deactivate();
	OnDeathDelegate.Broadcast(this);
}

void AASCharacterBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsStunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f : BaseWalkSpeed;
}

void AASCharacterBase::OnRep_Ignited()
{
}

void AASCharacterBase::OnRep_Stunned()
{
}

void AASCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
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

FOnASCRegistered& AASCharacterBase::GetOnASCRegisteredDelegate()
{
	return OnAscRegistered;
}

FOnDeathSignature& AASCharacterBase::GetOnDeathDelegate()
{
	return OnDeathDelegate;
}

USkeletalMeshComponent* AASCharacterBase::GetWeapon_Implementation()
{
	return Weapon;
}

FOnDamageSignature& AASCharacterBase::GetOnDamageSignature()
{
	return OnDamageDelegate;
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
