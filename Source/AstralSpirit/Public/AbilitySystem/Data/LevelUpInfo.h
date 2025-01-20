// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FASLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelUpRequirement = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 AttributePointReward = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 AbilityPointReward = 1;
	
};

/**
 * 
 */
UCLASS()
class ASTRALSPIRIT_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LevelUp")
	TArray<FASLevelUpInfo> LevelUpInformation;

	int32 FindLevelForXP(float XP);
	
};
