// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ASFireWave.h"

#include "GameFramework/ProjectileMovementComponent.h"

AASFireWave::AASFireWave()
{
	ProjectileMovement->PrimaryComponentTick.bCanEverTick = false;
	ProjectileMovement->SetAutoActivate(false);
}

void AASFireWave::BeginPlay()
{
	Super::BeginPlay();
}

void AASFireWave::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}
