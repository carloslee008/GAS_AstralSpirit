// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ASWaveOfFire.h"

FString UASWaveOfFire::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);;
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

		return FString::Printf(TEXT(
				// Title
				"<Title>Fire Blast</>\n\n"
				// Level
				"Level: <Level>%d</>\n"
				// Mana Cost
				"Mana Cost: <ManaCost>%.1f</>\n"
				// Cooldown
				"Cooldown: <Cooldown>%.1f</>\n\n"
				// Description
				"<Small>Unleashes a wave of %d fire balls, launching in all directions</>"
				"and returning once maximum range is reached, dealing"
				"<Damage>%d</><Small> fire damage</> upon impact"),

				// Values
				Level,
				ManaCost,
				Cooldown,
				NumFireBalls,
				ScaledDamage);
}

FString UASWaveOfFire::GetNextLevelDescription(int32 Level)
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
			"<Small>Unleashes a wave of %d fire balls, launching in all directions</>"
			"and returning once maximum range is reached, dealing"
			"<Damage>%d</><Small> fire damage</> upon impact"),

			// Values
			Level,
			ManaCost,
			Cooldown,
			NumFireBalls,
			ScaledDamage);
}

TArray<AASFireWave*> UASWaveOfFire::SpawnFireWaves()
{
	return TArray<AASFireWave*>();
}
