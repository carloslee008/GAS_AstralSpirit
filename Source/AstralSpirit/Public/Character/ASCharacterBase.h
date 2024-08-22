// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ASCharacterBase.generated.h"

UCLASS(Abstract)

class ASTRALSPIRIT_API AASCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AASCharacterBase();

protected:
	virtual void BeginPlay() override;

};
