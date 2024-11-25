// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "ASGameplayTags.h"
#include "AbilitySystem/ASAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	const UASAttributeSet* AS = CastChecked<UASAttributeSet>(AttributeSet);

	check(AttributeInfo);

	FASAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FASGameplayTags::Get().Attributes_Primary_Strength);
	Info.AttributeValue = AS->GetStrength();
	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
}
