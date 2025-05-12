// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ASEnemySpawnPoint.h"

#include "Character/ASEnemy.h"

void AASEnemySpawnPoint::SpawnEnemy()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AASEnemy* Enemy = GetWorld()->SpawnActorDeferred<AASEnemy>(EnemyClass, GetActorTransform());
	Enemy->SetLevel(EnemyLevel);
	Enemy->SetCharacterClass(CharacterClass);
	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController();
}
