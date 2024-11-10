// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASEffectActor.generated.h"

class UGameplayEffect;
class USphereComponent;

UCLASS()
class ASTRALSPIRIT_API AASEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AASEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToActor(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass);
	
	UPROPERTY(EditAnywhere, Category="Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectType;

};
