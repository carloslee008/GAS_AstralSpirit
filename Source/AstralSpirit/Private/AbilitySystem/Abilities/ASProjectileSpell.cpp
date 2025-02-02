// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ASProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "ASGameplayTags.h"
#include "Actor/ASProjectile.h"
#include "Interaction/CombatInterface.h"

FString UASProjectileSpell::GetDescription(int32 Level)
{
	const int32 Damage = DamageTypes[FASGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
				// Title
				"<Title>Fire Bolt</>\n"
				// Level
				"Level: <Level>%d</>\n"
				// Mana Cost

				// Cooldown

				// Description
				"<Small> Launches a bolt of fire, exploding on impact and dealing: </>"
				"<Damage>%d</><Small> fire damage</>"),

				// Values
				Level,
				Damage);
	}
	return FString::Printf(TEXT(
			// Title
			"<Title>Fire Bolt</>\n"
			// Level
			"Level: <Level>%d</>\n"
			// Mana Cost

			// Cooldown

			// Description
			"<Small> Launches %d bolts of fire, exploding on impact and dealing: </>"
			"<Damage>%d</><Small> fire damage</>"),

			// Values
			Level,
			FMath::Min(Level, MaxNumProjectiles),
			Damage);
}

FString UASProjectileSpell::GetNextLevelDescription(int32 Level)
{
	const int32 Damage = DamageTypes[FASGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
	return FString::Printf(TEXT(
			// Title
			"<Title>Next Level: </>\n"
			// Level
			"Level: <Level>%d</>\n"
			// Mana Cost

			// Cooldown

			// Description
			"<Small> Launches %d bolts of fire, exploding on impact and dealing: </>"
			"<Damage>%d</><Small> fire damage</>"),

			// Values
			Level,
			FMath::Min(Level, MaxNumProjectiles),
			Damage);
}

void UASProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UASProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
	bool bOverridePitch, float PitchOverride)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	// Socket location
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);

	// Rotation of Projectile
	FRotator SpawnRotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		SpawnRotation.Pitch = PitchOverride;
	}
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(SpawnRotation.Quaternion());
	
	AASProjectile* Projectile = GetWorld()->SpawnActorDeferred<AASProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	const UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();

	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(Projectile);
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(Projectile);
	EffectContextHandle.AddActors(Actors);
	FHitResult HitResult;
	HitResult.Location = ProjectileTargetLocation;
	EffectContextHandle.AddHitResult(HitResult);
		
	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

	const FASGameplayTags GameplayTags = FASGameplayTags::Get();

	for (auto& Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
	}

	Projectile->DamageEffectSpecHandle = SpecHandle;
	
	Projectile->FinishSpawning(SpawnTransform);
	
}
