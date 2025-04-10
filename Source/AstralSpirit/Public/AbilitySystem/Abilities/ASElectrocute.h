// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ASBeamSpell.h"
#include "ASElectrocute.generated.h"

/**
 * 
 */
UCLASS()
class ASTRALSPIRIT_API UASElectrocute : public UASBeamSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	
};
