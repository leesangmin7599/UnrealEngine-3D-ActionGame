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
		UUserWidget* WidgetReference; //�巡�׽� ������ ����
public:
	FSlotData SlotData; //���� ������
	FVector2D* DropOffset; //��ġ
};
