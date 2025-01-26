// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ASHUD.h"

#include "UI/Widget/ASUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/SkillMenuWidgetController.h"

UOverlayWidgetController* AASHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AASHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

USkillMenuWidgetController* AASHUD::GetSkillMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (SkillMenuWidgetController == nullptr)
	{
		SkillMenuWidgetController = NewObject<USkillMenuWidgetController>(this, SkillMenuWidgetControllerClass);
		SkillMenuWidgetController->SetWidgetControllerParams(WCParams);
		SkillMenuWidgetController->BindCallbacksToDependencies();
	}
	return SkillMenuWidgetController;
}

void AASHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class initialized, please fill out BP_ASHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller class uninitialized, please fill out BP_ASHUD"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UASUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	Widget->AddToViewport();
}
