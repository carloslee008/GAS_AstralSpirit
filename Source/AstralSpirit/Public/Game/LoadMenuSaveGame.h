// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoadMenuSaveGame.generated.h"

UENUM()
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

/**
 * 
 */
UCLASS()
class ASTRALSPIRIT_API ULoadMenuSaveGame : public USaveGame
{
	GENERATED_BODY()
public:

	UPROPERTY()
	FString SlotName = FString();

	UPROPERTY()
	int32 SlotIndex = 0;

	UPROPERTY()
	FString PlayerName = FString("Default Name");

	UPROPERTY()
	FString MapName = FString("Default Map Name");

	UPROPERTY()
	FName PlayerStartTag; // Note: FNames cannot contain spaces

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = Vacant;
};
