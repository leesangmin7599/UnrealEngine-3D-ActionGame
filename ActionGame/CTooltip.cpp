// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "CTooltip.h"



void UCTooltip::NativeConstruct()
{
	Super::NativeConstruct();

}
void UCTooltip::SetSlotData(FSlotData _SlotData)
{
	SlotData = _SlotData;
	//UE_LOG(LogTemp, Error, TEXT("CTooltip/SetSlotData Amout : %d"), SlotData.Amount); //1444372896
	NameText->SetText(FText::FromString(SlotData.ItemData.ItemName));
	PriceText->SetText(FText::AsNumber(SlotData.ItemData.Price));
	AmountText->SetText(FText::AsNumber(SlotData.Amount));
	
	DescriptionText->SetText(FText::FromString(SlotData.ItemData.Description));
	IconImage->SetBrushFromTexture(SlotData.ItemData.Icon);
	IconImage->SetBrushSize(FVector2D(70.f, 70.f));
}
