// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ASLightningBolt.h"

FString UASLightningBolt::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);;
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
			// Title
			"<Title>Lightning Bolt</>\n\n"
			// Level
			"Level: <Level>%d</>\n"
			// Mana Cost
			"Mana Cost: <ManaCost>%.1f</>\n"
			// Cooldown
			"Cooldown: <Cooldown>%.1f</>\n\n"
			// Description
			"<Small>Launches a bolt of lightning, exploding on impact and dealing: </>"
			"<Damage>%d</><Small> lightning damage</>"),

			// Values
			Level,
			ManaCost,
			Cooldown,
			ScaledDamage);
}

FString UASLightningBolt::GetNextLevelDescription(int32 Level)
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
			"<Small>Launches a bolt of lightning, exploding on impact and dealing: </>"
			"<Damage>%d</><Small> lightning damage</>"),

			// Values
			Level,
			ManaCost,
			Cooldown,
			ScaledDamage);
}
