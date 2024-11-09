// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ASEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/ASAttributeSet.h"
#include "Components/SphereComponent.h"

// Sets default values
AASEffectActor::AASEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);	
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
	
}

void AASEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO: Change this to apply Gameplay Effect. For now, using const_cast as a hack!
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UASAttributeSet* ASAttributeSet = Cast<UASAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UASAttributeSet::StaticClass()));
		UASAttributeSet* MutableASAttributeSet = const_cast<UASAttributeSet*>(ASAttributeSet);
		MutableASAttributeSet->SetHealth(ASAttributeSet->GetHealth() + 25.f);
		MutableASAttributeSet->SetMana(ASAttributeSet->GetMana() - 25.f);
		Destroy();
	}
}

void AASEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void AASEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AASEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AASEffectActor::EndOverlap);
}


