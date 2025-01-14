// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ASSummonAbility.h"

#include "Character/ASCharacterBase.h"

TArray<FVector> UASSummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector(); // Unit Vector
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions;

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2, FVector::UpVector);
	TArray<FVector> SpawnLocations;
	for (int32 i = 0; i < NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		FVector RandomSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult Hit;
		// Line trace from above, so we get the spawn locations on ground in case of a sloped ground.
		GetWorld()->LineTraceSingleByChannel(Hit, RandomSpawnLocation + FVector(0.f, 0.f, 400.f), RandomSpawnLocation - FVector(0.f, 0.f,400.f), ECC_Visibility); 
		if (Hit.bBlockingHit)
		{
			RandomSpawnLocation = Hit.ImpactPoint;
		}
		SpawnLocations.Add(RandomSpawnLocation);
	}
	return SpawnLocations;
}

AASCharacterBase* UASSummonAbility::GetRandomMinionClassCDO()
{
	if (MinionClasses.Num() == 0) return nullptr;
	const int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[Selection]->GetDefaultObject<AASCharacterBase>();
}
