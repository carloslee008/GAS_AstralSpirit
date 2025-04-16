// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ASFireWave.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ASGameplayTags.h"
#include "GameplayCueManager.h"
#include "AbilitySystem/ASAbilitySystemBlueprintLibrary.h"
#include "Components/AudioComponent.h"

void AASFireWave::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}

void AASFireWave::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;
	if (IgnoreList.Contains(OtherActor)) return;
	
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			// Set Death Impulse
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
			
			// Set Target ASC
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			// Apply Damage
			UASAbilitySystemBlueprintLibrary::ApplyDamageEffect(DamageEffectParams);
			IgnoreList.Add(OtherActor);
		}
	}
}

void AASFireWave::OnHit()
{
	if (GetOwner())
	{
		FGameplayCueParameters CueParams;
		CueParams.Location = GetActorLocation();
		// Execute GameplayCue locally
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FASGameplayTags::Get().GameplayCue_WaveOfFire, CueParams);
	}
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	bHit = true;
}

void AASFireWave::EmptyIgnoreList()
{
	IgnoreList.Empty();
}
