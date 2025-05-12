// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ASEnemySpawnVolume.h"

#include "Actor/ASEnemySpawnPoint.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"

// Sets default values
AASEnemySpawnVolume::AASEnemySpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box");
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionObjectType(ECC_WorldStatic);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}

void AASEnemySpawnVolume::LoadActor_Implementation()
{
	if (bReached)
	{
		BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

// Called when the game starts or when spawned
void AASEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AASEnemySpawnVolume::OnBoxOverlap);
	
}

void AASEnemySpawnVolume::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UPlayerInterface>()) return;
	bReached = true;
	for (AASEnemySpawnPoint* Point : SpawnPoints)
	{
		if (IsValid(Point))
		{
			Point->SpawnEnemy();
		}
	}
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

