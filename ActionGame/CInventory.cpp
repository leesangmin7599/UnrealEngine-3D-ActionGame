// Fill out your copyright notice in the Description page of Project Settings.

#include "CSlot.h"
#include "CInventory.h"

void UCInventory::NativeConstruct()
{
	Super::NativeConstruct();
	for (int i = 0; i < 30; i++)
	{
		FName slotName = FName(FString::Printf(TEXT("WB_Slot_%d"), i));
		UCSlot* slot = Cast<UCSlot>(GetWidgetFromName(slotName));
		if (slot == nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("NotSlot!"));
		}
		Slots.Add(slot);
	}
}

void UCInventory::AddSlotToItem(FItemData _ItemData)
{
	for (UCSlot* item : Slots)
	{
		if (item->SlotData.ItemData == _ItemData && _ItemData.Type == EItemType::Equipment)
		{
			continue;
		}
		else if (item->SlotData.Amount == 0)
		{
			item->SlotData.ItemData = _ItemData;
			item->SlotData.Amount++;
			item->SetSlotData(item->SlotData);
			return;
		}
		else if (item->SlotData.ItemData == _ItemData)
		{
			item->SlotData.Amount += 1;
			item->SetSlotData(item->SlotData);
			return;
		}
	}
}

