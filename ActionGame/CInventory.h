// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemData.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CInventory.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UCInventory : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;//AddToViewport할때마다 호출되는 함수
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory")
		TArray<class UCSlot*> Slots;
public:
	void AddSlotToItem(FItemData _ItemData);

};
