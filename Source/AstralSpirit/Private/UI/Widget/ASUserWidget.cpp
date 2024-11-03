// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/ASUserWidget.h"

void UASUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
