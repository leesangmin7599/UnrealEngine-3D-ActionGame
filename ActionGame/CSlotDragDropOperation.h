// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemData.h"
#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "CSlotDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UCSlotDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DragDrop")
		UUserWidget* WidgetReference; //드래그시 보여줄 위젯
public:
	FSlotData SlotData; //슬롯 데이터
	FVector2D* DropOffset; //위치
};
