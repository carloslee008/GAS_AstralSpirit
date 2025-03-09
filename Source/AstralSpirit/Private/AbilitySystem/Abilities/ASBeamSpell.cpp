// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ASBeamSpell.h"

#include "AbilitySystem/ASAbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UASBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}

void UASBeamSpell::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UASBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter);
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		if (USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(OwnerCharacter))
		{
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(OwnerCharacter);
			FHitResult HitResult;
			const FVector SocketLocation = Weapon->GetSocketLocation(FName("TipSocket"));
			UKismetSystemLibrary::SphereTraceSingle(OwnerCharacter,
				SocketLocation,
				BeamTargetLocation,
				10.f,
				TraceTypeQuery1,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::None,
				HitResult,
				true);

			if (HitResult.bBlockingHit)
			{
				MouseHitLocation = HitResult.ImpactPoint;
				MouseHitActor = HitResult.GetActor();
			}
		}
	}
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(MouseHitActor))
	{
		// Only bind if not already bound
		if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UASBeamSpell::PrimaryTargetDied))
		{
			CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UASBeamSpell::PrimaryTargetDied);
		}
	}
}

void UASBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(MouseHitActor);
	
	TArray<AActor*> OverlappingActors;
	
	UASAbilitySystemBlueprintLibrary::GetLivePlayersWithinRadius(
		GetAvatarActorFromActorInfo(),
		OverlappingActors,
		ActorsToIgnore,
		850.f,
		MouseHitActor->GetActorLocation());

	int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel() - 1, MaxNumShockTargets);
	

	UASAbilitySystemBlueprintLibrary::GetClosestTargets(
		NumAdditionalTargets,
		OverlappingActors,
		OutAdditionalTargets,
		MouseHitActor->GetActorLocation());

	for (AActor* Target : OutAdditionalTargets)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target))
		{
			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UASBeamSpell::AdditionalTargetDied))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UASBeamSpell::AdditionalTargetDied);
			}
		}
	}
}

void UASBeamSpell::RemoveOnDeathNotify(AActor* Actor)
{
	if (const auto CombatInterface = Cast<ICombatInterface>(Actor))
	{
		if (CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &ThisClass::AdditionalTargetDied))
		{
			CombatInterface->GetOnDeathDelegate().RemoveDynamic(this, &ThisClass::AdditionalTargetDied);
		}
		if (CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &ThisClass::PrimaryTargetDied))
		{
			CombatInterface->GetOnDeathDelegate().RemoveDynamic(this, &ThisClass::PrimaryTargetDied);
		}
	}
}
