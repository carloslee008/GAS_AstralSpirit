// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ASFireBolt.h"

#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

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
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-ProjectileSpread / 2.f, FVector::UpVector);
	const FVector RightOfSpread = Forward.RotateAngleAxis(ProjectileSpread / 2.f, FVector::UpVector);

	// const int32 NumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());
	if (MaxNumProjectiles > 1)
	{
		const float DeltaSpread = ProjectileSpread / (MaxNumProjectiles - 1);
		for (int32 i = 0; i < MaxNumProjectiles; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			const FVector Start = SocketLocation + FVector(0,0,5);
			UKismetSystemLibrary::DrawDebugArrow(
				GetAvatarActorFromActorInfo(),
				Start,
				Start + Direction * 75.f,
				1,
				FLinearColor::Red,
				120,
				1);

		}
	}
	else
	{
		// Single Projectile
		const FVector Start = SocketLocation + FVector(0,0,5);
		UKismetSystemLibrary::DrawDebugArrow(
				GetAvatarActorFromActorInfo(),
				Start,
				Start + Forward * 75.f,
				1,
				FLinearColor::Red,
				120,
				1);
	}
	
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + Forward * 100.f, 1, FLinearColor::White, 120, 1);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + LeftOfSpread * 100.f, 1, FLinearColor::Gray, 120, 1);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + RightOfSpread * 100.f, 1, FLinearColor::Gray, 120, 1);


}
