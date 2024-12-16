// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ASAbilitySystemGlobals.h"

#include "ASAbilityTypes.h"

FGameplayEffectContext* UASAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FASGameplayEffectContext;
}
