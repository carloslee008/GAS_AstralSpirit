// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "ASGameplayTags.h"
#include "AbilitySystem/ASAbilitySystemComponent.h"
#include "AbilitySystem/ASAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/ASPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetASAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetASAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetASAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetASAS()->GetMaxMana());
	OnXPPercentChangedDelegate.Broadcast(GetASPS()->GetPlayerXP());
}

// Configure Broadcast events to update the overlay widget components
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetASPS()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	GetASPS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel, bool bLevelUp)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel, bLevelUp);
		}	
	);

	/* 
	 *	Vital Attributes
	 */
	
	// Health
	BindAttributeChange(GetASAS()->GetHealthAttribute(), &OnHealthChanged);
	
	// Max Health
	BindAttributeChange(GetASAS()->GetMaxHealthAttribute(), &OnMaxHealthChanged);
	
	// Mana
	BindAttributeChange(GetASAS()->GetManaAttribute(), &OnManaChanged);
	
	// Max Mana
	BindAttributeChange(GetASAS()->GetMaxManaAttribute(), &OnMaxManaChanged);
	
	if (GetASASC())
	{
		GetASASC()->AbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		if (GetASASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetASASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}
		
		GetASASC()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)
				{
					// For example, let's say Tag = Message.HealthPotion
					// "Message.HealthPotion".MatchesTag("Message") will return True,
					// "Message".MatchesTag("Message.HealthPotion") will return False
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag))
					{
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetASPS()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Cannot find LevelUpInfo. Please fill out ASPlayerState Blueprint."));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num() - 1;

	if (Level < MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

		const int32 CurrentLevelXPRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForCurrentLevel = NewXP - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForCurrentLevel) / static_cast<float>(CurrentLevelXPRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
	
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
	const FASGameplayTags& GameplayTags = FASGameplayTags::Get();

	// Clear out the old slot
	
	FASAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	// Broadcast empty info if PreviousSlot is a valid slot. Only if equipping an already-equipped skill
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	// Fill in the new slot
	
	FASAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	// Broadcast current slot
	AbilityInfoDelegate.Broadcast(Info);
}

void UOverlayWidgetController::BindAttributeChange(const FGameplayAttribute& Attribute,
                                                   FOnAttributeChangedSignature* AttributeChangeDelegate) const
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddLambda(
		[AttributeChangeDelegate](const FOnAttributeChangeData& Data)
		{
			AttributeChangeDelegate->Broadcast(Data.NewValue);
		}
	);
}
