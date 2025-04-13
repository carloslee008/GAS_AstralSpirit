// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ASDamageGameplayAbility.h"
#include "ASWaveOfFire.generated.h"

class AASFireWave;
/**
 * 
 */
UCLASS()
class ASTRALSPIRIT_API UASWaveOfFire : public UASDamageGameplayAbility
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	TArray<AASFireWave*> SpawnFireWaves();

protected:

	UPROPERTY(EditDefaultsOnly, Category="FireBlast")
	int32 NumFireBalls = 10;

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AASFireWave> FireWaveClass;
	
};
