// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Engine/TargetPoint.h"
#include "ASEnemySpawnPoint.generated.h"

class AASEnemy;
/**
 * 
 */
UCLASS()
class ASTRALSPIRIT_API AASEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy Class")
	TSubclassOf<AASEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy Class")
	int32 EnemyLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy Class")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
};
