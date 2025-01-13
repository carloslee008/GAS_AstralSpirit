// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ASGameplayAbility.h"
#include "ASSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class ASTRALSPIRIT_API UASSummonAbility : public UASGameplayAbility
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();

	UPROPERTY(EditAnywhere, Category="Summoning")
	int32 NumMinions = 5;

	UPROPERTY(EditAnywhere, Category="Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditAnywhere, Category="Summoning")
	float MinSpawnDistance = 75.f;
	
	UPROPERTY(EditAnywhere, Category="Summoning")
	float MaxSpawnDistance = 400.f;

	UPROPERTY(EditAnywhere, Category="Summoning")
	float SpawnSpread = 90.f;
	
};
