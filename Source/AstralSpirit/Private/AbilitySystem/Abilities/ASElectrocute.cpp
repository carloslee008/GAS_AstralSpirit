// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ASElectrocute.h"

FString UASElectrocute::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);;
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
				// Title
				"<Title>Electrocute</>\n\n"
				// Level
				"Level: <Level>%d</>\n"
				// Mana Cost
				"Mana Cost: <ManaCost>%.1f</>\n"
				// Cooldown
				"Cooldown: <Cooldown>%.1f</>\n\n"
				// Description
				"<Small>Emits a beam of lightning,"
				"connecting with the target, repeatedly causing </>"
				"<Damage>%d</><Small> lightning damage with"
				"a chance to stun</>"),

				// Values
				Level,
				ManaCost,
				Cooldown,
				ScaledDamage);
	}
	return FString::Printf(TEXT(
			// Title
			"<Title>Electrocute</>\n\n"
			// Level
			"Level: <Level>%d</>\n"
			// Mana Cost
			"Mana Cost: <ManaCost>%.1f</>\n"
			// Cooldown
			"Cooldown: <Cooldown>%.1f</>\n\n"
			// Description
			"<Small>Emits a beam of lightning, "
			"propogating to %d additional nearby targets, causing</>"
			"<Damage>%d</><Small> lightning damage</>"),

			// Values
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level - 1, MaxNumShockTargets),
			ScaledDamage);
}

FString UASElectrocute::GetNextLevelDescription(int32 Level)
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
			"<Small>Emits a beam of lightning, "
			"propogating to %d additional nearby targets, causing</>"
			"<Damage>%d</><Small> lightning damage</>"),

			// Values
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level -1, MaxNumShockTargets),
			ScaledDamage);
}
