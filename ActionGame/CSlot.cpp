// Fill out your copyright notice in the Description page of Project Settings.

#include "CSlotDragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "CItem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/ScaleBox.h"
#include "CTooltip.h"
#include "Components/Image.h"
#include "CSlot.h"

UCSlot::UCSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Super::NativeOnInitialized();
	ConstructorHelpers::FClassFinder<UUserWidget> TooltipAsset(TEXT("WidgetBlueprint'/Game/Blueprints/Player/WB_Tooltip.WB_Tooltip_C'"));
	if (TooltipAsset.Succeeded())
	{
		TooltipClass = TooltipAsset.Class;
	}
}

void UCSlot::NativeConstruct()
{
	Super::NativeConstruct();
	if (SlotData.ItemData.Icon)
	{
		Icon->SetBrushFromTexture(SlotData.ItemData.Icon);
	}
	else
	{
		Icon->SetBrushFromTexture(nullptr);
	}
	Tooltip = Cast<UCTooltip>(CreateWidget(GetWorld(), TooltipClass));
	Tooltip->SetSlotData(SlotData);
}

void UCSlot::SetSlotData(FSlotData _SlotData)
{
	SlotData = _SlotData;
	//UE_LOG(LogTemp, Error, TEXT("CSlot/SetSlotData Amout : %d"), SlotData.Amount); //1444372896
	if(SlotData.ItemData.Type != EItemType::None)
	{
		Icon->SetBrushFromTexture(SlotData.ItemData.Icon);
		Icon->SetBrushSize(FVector2D(100, 100));
		SlotScaleBox->SetToolTip(Tooltip);
	}
	else
	{
		Icon->SetBrushFromTexture(nullptr);
		SlotScaleBox->SetToolTip(nullptr);
	}
	Tooltip->SetSlotData(SlotData);
}

FReply UCSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (InMouseEvent.IsMouseButtonDown(FKey("LeftMouseButton")))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("LeftDown"));
		reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		//DetectDragIfPressed : 버튼이 눌린상태로 드래그했는지 판단하는 함수
	}
	if (InMouseEvent.IsMouseButtonDown(FKey("RightMouseButton")))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("RightDown"));
		if (SlotData.ItemData.Type == EItemType::Countable)
		{
			SlotData.Amount--;
			Tooltip->SetSlotData(SlotData);
			if (SlotData.Amount <= 0)
			{
				FSlotData slotData;
				SlotData = slotData;
				SetSlotData(SlotData);
				Icon->SetBrushFromTexture(nullptr);
				SlotScaleBox->SetToolTip(nullptr); 
			}
		}
	}
	return reply.NativeReply;
}

void UCSlot::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	SetVisibility(ESlateVisibility::Visible);
}



void UCSlot::SpawnItem()
{
	ACharacter* character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FTransform transform = character->GetActorTransform();
	transform.SetLocation(character->GetActorLocation() + (character->GetActorForwardVector() * 200));
	ACItem* item = GetWorld()->SpawnActorDeferred<ACItem>(ACItem::StaticClass(), transform);
	//SpawnActorDeferred : 액터 생성 후 월드에 추가하지 않음
	item->SetItemRowName(FName(*SlotData.ItemData.ItemName));
	item->FinishSpawning(transform); //월드에 아이템 추가
	FSlotData tempData;
	SlotData = tempData;
	SetSlotData(tempData);
}

void UCSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	if (SlotData.ItemData.Icon == nullptr)
		return;
	UCSlotDragDropOperation* DragDrop = NewObject<UCSlotDragDropOperation>();
	SetVisibility(ESlateVisibility::HitTestInvisible);
	DragDrop->SlotData = SlotData;
	DragDrop->WidgetReference = this;
	DragDrop->DefaultDragVisual = this; //드래그 시 표시될 위젯
	DragDrop->Pivot = EDragPivot::MouseDown; //드래그시 위젯이 표시되는 위치
	OutOperation = DragDrop; //오퍼레이션 적용
}

bool UCSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UCSlotDragDropOperation* DragDrop = Cast<UCSlotDragDropOperation>(InOperation);
	
	if(DragDrop->WidgetReference != this)
	{
		SetSlotData(DragDrop->SlotData);
		UCSlot* slot = Cast<UCSlot>(DragDrop->WidgetReference);
		FSlotData tempData;
		if (slot != nullptr)
		{
			slot->SetSlotData(tempData);
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("this!"));
	DragDrop->WidgetReference->SetVisibility(ESlateVisibility::Visible);
	SetVisibility(ESlateVisibility::Visible);
	return false;
}
