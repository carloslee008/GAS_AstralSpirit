// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ASIceShards.h"

FString UASIceShards::GetDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);;
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
				// Title
				"<Title>Ice Shards</>\n\n"
				// Level
				"Level: <Level>%d</>\n"
				// Mana Cost
				"Mana Cost: <ManaCost>%.1f</>\n"
				// Cooldown
				"Cooldown: <Cooldown>%.1f</>\n\n"
				// Description
				"<Small>Summons a shard of ice,"
				"causing radial ice damage of </>"
				"<Damage>%d</><Small> at the shard"
				"origin</>"),

				// Values
				Level,
				ManaCost,
				Cooldown,
				ScaledDamage);
	}
	return FString::Printf(TEXT(
			// Title
			"<Title>Ice Shards</>\n\n"
			// Level
			"Level: <Level>%d</>\n"
			// Mana Cost
			"Mana Cost: <ManaCost>%.1f</>\n"
			// Cooldown
			"Cooldown: <Cooldown>%.1f</>\n\n"
			// Description
			"<Small>Summons %d shards of ice,"
			"causing radial ice damage of </>"
			"<Damage>%d</><Small> at the shard"
			"origin</>"),

			// Values
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, MaxNumShards),
			ScaledDamage);
}

FString UASIceShards::GetNextLevelDescription(int32 Level)
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
			"<Small>Summons %d shards of ice,"
			"causing radial ice damage of </>"
			"<Damage>%d</><Small> at the shard"
			"origin</>"),

			// Values
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, MaxNumShards),
			ScaledDamage);
}
