// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ASAIController.generated.h"

class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class ASTRALSPIRIT_API AASAIController : public AAIController
{
	GENERATED_BODY()
public:
	AASAIController();
protected:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
