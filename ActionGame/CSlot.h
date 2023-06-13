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
	virtual void NativeConstruct() override;//AddToViewport�Ҷ����� ȣ��Ǵ� �Լ�
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
	//�������ο��� ���콺 Ŭ���� ȣ��Ǵ� �Լ�
	//FGeometry : ������ ��ġ, ũ��, ������ġ�� ��Ÿ�� ������ġ�� Geometry�� ���۵� ��ġ�� ���� ȭ�����
	//FPointerEvent : ���콺 �Ǵ� ��ġ�� ���� �̺�Ʈ �ڵ鷯�� ����
	//FEventReply : ����ڰ� �̺�Ʈ�� ó���ϰ� �⺻ UI������ ������ ��ȯ�ϸ� ��ȯ�� ������ ���� ����ü
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
private:
	UPROPERTY(VisibleAnywhere, Category = "Item")
		TSubclassOf<class ACItem> Item;
public:
	void SpawnItem();
private:
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	//DragDropOperation�� ����Ϸ��� Override�ؾ���
	//�巡���ϸ� ȣ��Ǵ� �Լ�

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

};
