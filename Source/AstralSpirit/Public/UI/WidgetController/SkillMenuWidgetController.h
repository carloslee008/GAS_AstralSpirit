// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ASGameplayTags.h"
#include "UI/WidgetController/ASWidgetController.h"
#include "GameplayTagContainer.h"
#include "SkillMenuWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSkillSelectedSignature, bool, bSkillPointsButtonEnabled, bool,
                                             bEquippedButtonEnabled);

struct FSelectedSkill
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class ASTRALSPIRIT_API USkillMenuWidgetController : public UASWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature SkillPointsChanged;

	UPROPERTY(BlueprintAssignable)
	FSkillSelectedSignature SkillSelectedDelegate;

	UFUNCTION(BlueprintCallable)
	void SkillSelected(const FGameplayTag& AbilityTag);

private:

	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SkillPoints, bool& bShouldEnableSkillPointsButton, bool& bShouldEnableEquipButton);
	FSelectedSkill SelectedSkill = { FASGameplayTags::Get().Abilities_None, FASGameplayTags::Get().Abilities_Status_Locked };
	int32 CurrentSkillPoints = 0;
	
};
