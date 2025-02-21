// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ASFireBolt.h"

#include "AbilitySystem/ASAbilitySystemBlueprintLibrary.h"
#include "Actor/ASProjectile.h"
#include "Interaction/CombatInterface.h"

FString UASFireBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);;
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
				// Title
				"<Title>Fire Bolt</>\n\n"
				// Level
				"Level: <Level>%d</>\n"
				// Mana Cost
				"Mana Cost: <ManaCost>%.1f</>\n"
				// Cooldown
				"Cooldown: <Cooldown>%.1f</>\n\n"
				// Description
				"<Small>Launches a bolt of fire, exploding on impact and dealing: </>"
				"<Damage>%d</><Small> fire damage</>"),

				// Values
				Level,
				ManaCost,
				Cooldown,
				ScaledDamage);
	}
	return FString::Printf(TEXT(
			// Title
			"<Title>Fire Bolt</>\n\n"
			// Level
			"Level: <Level>%d</>\n"
			// Mana Cost
			"Mana Cost: <ManaCost>%.1f</>\n"
			// Cooldown
			"Cooldown: <Cooldown>%.1f</>\n\n"
			// Description
			"<Small>Launches %d bolts of fire, exploding on impact and dealing: </>"
			"<Damage>%d</><Small> fire damage</>"),

			// Values
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, MaxNumProjectiles),
			ScaledDamage);
}

FString UASFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);;
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			// Title
			"<Title>Next Level: </>\n\n"
			// Level
			"Level: <Level>%d</>\n"
			// Mana Cost
			"Mana Cost: <ManaCost>%.1f</>\n"
			// Cooldown
			"Cooldown: <Cooldown>%.1f</>\n\n"
			// Description
			"<Small>Launches %d bolts of fire, exploding on impact and dealing: </>"
			"<Damage>%d</><Small> fire damage</>"),

			// Values
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, MaxNumProjectiles),
			ScaledDamage);
}

void UASFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
	bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	// Socket location
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);

	// Rotation of Projectile
	FRotator SpawnRotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch) SpawnRotation.Pitch = PitchOverride;

	const FVector Forward = SpawnRotation.Vector();

	TArray<FRotator> Rotations = UASAbilitySystemBlueprintLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, MaxNumProjectiles);
	
	for (const FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());

		AASProjectile* Projectile = GetWorld()->SpawnActorDeferred<AASProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	
		Projectile->FinishSpawning(SpawnTransform);
		
	}
}
