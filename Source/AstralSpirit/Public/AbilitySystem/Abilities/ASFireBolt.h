// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ASProjectileSpell.h"
#include "ASFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class ASTRALSPIRIT_API UASFireBolt : public UASProjectileSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	
};
