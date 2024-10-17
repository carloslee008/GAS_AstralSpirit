// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ASPlayerController.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS()
class ASTRALSPIRIT_API AASPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AASPlayerController();
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> ASContext;
};
