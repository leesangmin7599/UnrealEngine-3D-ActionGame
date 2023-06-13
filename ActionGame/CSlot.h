// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemData.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSlot.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UCSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	UCSlot(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void NativeConstruct() override;//AddToViewport할때마다 호출되는 함수
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Slot", meta = (BindWidget))
		class UImage* Icon;
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Slot", meta = (AllowPrivateAccess = true))
		FSlotData SlotData;
public:
	void SetSlotData(FSlotData _SlotData);
private:
	UPROPERTY(VisibleAnywhere, Category = "Tooltip")
		TSubclassOf<UUserWidget> TooltipClass;
	UPROPERTY(VisibleAnywhere, Category = "Tooltip")
		class UCTooltip* Tooltip;
	UPROPERTY(VisibleAnywhere, Category = "Tooltip", meta = (BindWidget))
		class UScaleBox* SlotScaleBox;
private:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//위젯내부에서 마우스 클릭시 호출되는 함수
	//FGeometry : 위젯의 위치, 크기, 절대위치를 나타냄 절대위치는 Geometry가 시작된 위치에 따른 화면공간
	//FPointerEvent : 마우스 또는 터치에 따른 이벤트 핸들러를 전달
	//FEventReply : 사용자가 이벤트를 처리하고 기본 UI계층에 정보를 반환하며 반환할 정보를 담은 구조체
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
private:
	UPROPERTY(VisibleAnywhere, Category = "Item")
		TSubclassOf<class ACItem> Item;
public:
	void SpawnItem();
private:
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	//DragDropOperation을 사용하려면 Override해야함
	//드래그하면 호출되는 함수

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

};
