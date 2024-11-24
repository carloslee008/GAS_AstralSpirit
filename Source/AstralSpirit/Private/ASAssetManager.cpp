// Fill out your copyright notice in the Description page of Project Settings.


#include "ASAssetManager.h"
#include "ASGameplayTags.h"

UASAssetManager& UASAssetManager::Get()
{
	check(GEngine);
	
	UASAssetManager* ASAssetManager = Cast<UASAssetManager>(GEngine->AssetManager);
	return *ASAssetManager;
}

void UASAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FASGameplayTags::InitializeNativeGameplayTags();
}
