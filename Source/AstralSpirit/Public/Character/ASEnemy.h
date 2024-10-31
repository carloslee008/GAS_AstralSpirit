// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ASCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "ASEnemy.generated.h"

/**
 * 
 */
UCLASS()
class ASTRALSPIRIT_API AASEnemy : public AASCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	AASEnemy();

	
public:
	//~ Begin Enemy Interface.
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//~ End Enemy Interface.

protected:
	virtual void BeginPlay() override;

};
