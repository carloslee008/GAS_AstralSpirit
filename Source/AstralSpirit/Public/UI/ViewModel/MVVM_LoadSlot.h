// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadMenuSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectSlotButton, bool, bEnable);


/**
 * 
 */
UCLASS()
class ASTRALSPIRIT_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;

	UPROPERTY(BlueprintAssignable)
	FEnableSelectSlotButton EnableSelectSlotButton;
	
	void InitializeSlot();

	UPROPERTY()
	int32 SlotIndex;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;

	UPROPERTY()
	FName PlayerStartTag;

	UPROPERTY()
	FString MapAssetName;

	/** Field Notifies */
	
	void SetPlayerName(FString InPlayerName);
	void SetLoadSlotName(FString InLoadSlotName);
	void SetMapName(FString InMapName);
	void SetPlayerLevel(int32 InLevel);

	FString GetPlayerName() const { return PlayerName; }
	FString GetLoadSlotName() const { return LoadSlotName; }
	FString GetMapName() const { return MapName; }
	int32 GetPlayerLevel() const { return PlayerLevel; }

private:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
	FString PlayerName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
	FString LoadSlotName;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(AllowPrivateAccess="true"))
	int32 PlayerLevel;

	
};
