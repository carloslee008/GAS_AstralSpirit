// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ASFireWave.h"

void AASFireWave::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}

void AASFireWave::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}
