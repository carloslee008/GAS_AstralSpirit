// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Checkpoint.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class ASTRALSPIRIT_API ACheckpoint : public APlayerStart, public ISaveInterface
{
	GENERATED_BODY()
public:
	ACheckpoint(const FObjectInitializer& ObjectInitializer);

	/* Save Interface */
	virtual bool ShouldLoadTransform_Implementation() override { return false; }
	virtual void LoadActor_Implementation() override;
	
	/* End Save Interface */

	UPROPERTY(BlueprintReadOnly, SaveGame) // SaveGame makes this serializable
	bool bReached = false;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void StartGlowTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	void Glow();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;
	
private:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
};
