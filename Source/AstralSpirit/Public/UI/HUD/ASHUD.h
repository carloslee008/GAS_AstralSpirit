// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ASHUD.generated.h"

class UASUserWidget;
/**
 * 
 */
UCLASS()
class ASTRALSPIRIT_API AASHUD : public AHUD
{
	GENERATED_BODY()
public:

	UPROPERTY()
	TObjectPtr<UASUserWidget> OverlayWidget;

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UASUserWidget> OverlayWidgetClass;
	
};
