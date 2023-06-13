// Fill out your copyright notice in the Description page of Project Settings.
#include "CSlot.h"
#include "CSlotDragDropOperation.h"
#include "CDrop.h"

bool UCDrop::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UCSlotDragDropOperation* DragDrop = Cast<UCSlotDragDropOperation>(InOperation);

	if (DragDrop->WidgetReference != this)
	{
		UCSlot* slot = Cast<UCSlot>(DragDrop->WidgetReference);
		slot->SpawnItem();
	}
	DragDrop->WidgetReference->SetVisibility(ESlateVisibility::Visible);
	return false;
}
