// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ASProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/ASProjectile.h"
#include "Interaction/CombatInterface.h"

void UASProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void UASProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		// Socket location
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();

		// Rotation of Projectile
		FRotator SpawnRotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		SpawnRotation.Pitch = 0;
		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(SpawnRotation.Quaternion());
		
		AASProjectile* Projectile = GetWorld()->SpawnActorDeferred<AASProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetAvatarActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		// TODO : Give the Projectile a Gameplay Effect Spec for causing Damage.
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();

		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
		Projectile->DamageEffectSpecHandle = SpecHandle;
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}
