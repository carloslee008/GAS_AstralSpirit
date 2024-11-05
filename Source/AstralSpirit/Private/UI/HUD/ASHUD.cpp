// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ASHUD.h"

#include "UI/Widget/ASUserWidget.h"

void AASHUD::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	Widget->AddToViewport();
}
