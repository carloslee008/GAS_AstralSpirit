// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ASCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "ASCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ASTRALSPIRIT_API AASCharacter : public AASCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
public:
	AASCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Player Interface */
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void AddToLevel_Implementation(int32 InLevel) override;
	/** End Player Interface */

	/** Begin Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() override;
	/** End Combat Interface */
private:
	virtual void InitAbilityActorInfo() override;
};
