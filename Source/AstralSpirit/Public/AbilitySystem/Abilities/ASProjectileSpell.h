// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ASGameplayAbility.h"
#include "ASProjectileSpell.generated.h"

class AASProjectile;
/**
 * 
 */
UCLASS()
class ASTRALSPIRIT_API UASProjectileSpell : public UASGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AASProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

};
