// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * ASGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */


struct FASGameplayTags
{
public:
 static const FASGameplayTags& Get() { return GameplayTags; }
 static void InitializeNativeGameplayTags();
protected:

private:
 static FASGameplayTags GameplayTags;
};
