// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/MagicCircle.h"

#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Interaction/HighlightInterface.h"

// Sets default values
AMagicCircle::AMagicCircle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MagicCircleSphere = CreateDefaultSubobject<USphereComponent>("MagicCircleSphere");
	SetRootComponent(MagicCircleSphere);
	MagicCircleSphere->SetCollisionResponseToChannels(ECR_Ignore);
	MagicCircleSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>("MagicCircleDecal");
	MagicCircleDecal->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();

	MagicCircleSphere->OnComponentBeginOverlap.AddDynamic(this, &AMagicCircle::OnSphereBeginOverlap);
	MagicCircleSphere->OnComponentEndOverlap.AddDynamic(this, &AMagicCircle::OnSphereEndOverlap);
	
}

void AMagicCircle::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IHighlightInterface* Enemy = Cast<IHighlightInterface>(OtherActor))
	{
		Enemy->Execute_HighlightActor(OtherActor);
	}
}

void AMagicCircle::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IHighlightInterface* Enemy = Cast<IHighlightInterface>(OtherActor))
	{
		Enemy->Execute_UnHighlightActor(OtherActor);
	}
}

void AMagicCircle::SetTargetingRadius(const float Radius) const
{
	MagicCircleSphere->SetSphereRadius(Radius);
	MagicCircleDecal->DecalSize = FVector(Radius);
}

// Called every frame
void AMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

