// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/ASWidgetController.h"
#include "SkillMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class ASTRALSPIRIT_API USkillMenuWidgetController : public UASWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature SkillPointsChanged;
	
};
