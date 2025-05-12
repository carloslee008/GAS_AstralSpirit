// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "ASEnemySpawnVolume.generated.h"

class AASEnemySpawnPoint;
class UBoxComponent;

UCLASS()
class ASTRALSPIRIT_API AASEnemySpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASEnemySpawnVolume();

	/* Save Interface */
	virtual void LoadActor_Implementation() override;
	/* End Save Interface */

	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	TArray<AASEnemySpawnPoint*> SpawnPoints;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;


};
