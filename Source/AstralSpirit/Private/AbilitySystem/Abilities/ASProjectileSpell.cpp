// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ASProjectileSpell.h"

#include "Kismet/KismetSystemLibrary.h"

void UASProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UKismetSystemLibrary::PrintString(this, FString("ActivateAbility (C++)"), true, true, FColor::Yellow, 3);
	
}
