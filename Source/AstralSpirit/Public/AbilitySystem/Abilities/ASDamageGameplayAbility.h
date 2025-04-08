// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASAbilityTypes.h"
#include "AbilitySystem/Abilities/ASGameplayAbility.h"
#include "ASDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ASTRALSPIRIT_API UASDamageGameplayAbility : public UASGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr, FVector InRadialDamageOrigin = FVector::ZeroVector) const;

	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffChance = 20.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DebuffDuration = 5.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float DeathImpulseMagnitude = 10000.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float KnockbackForceMagnitude = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float KnockbackChance = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	bool bIsRadialDamage = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat Damage;
};
