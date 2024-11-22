// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ASCharacterBase.h"
#include "ASCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ASTRALSPIRIT_API AASCharacter : public AASCharacterBase
{
	GENERATED_BODY()
public:
	AASCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	//~ Begin Combat Interface
	virtual int32 GetPlayerLevel() override;
	//~ End Combat Interface
private:
	virtual void InitAbilityActorInfo() override;
};
