// Fill out your copyright notice in the Description page of Project Settings.
//#include "Components/SlateWrapperTypes.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "CInventory.h"

#include "ItemData.h"
#include "CInventoryComponent.h"

// Sets default values for this component's properties
UCInventoryComponent::UCInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	static ConstructorHelpers::FClassFinder<UUserWidget> InventoryAsset(TEXT("WidgetBlueprint'/Game/Blueprints/Player/WB_Inventory.WB_Inventory_C'"));
	if (InventoryAsset.Succeeded())
	{
		InventoryClass = InventoryAsset.Class;
	}
}


// Called when the game starts
void UCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	if (InventoryClass != nullptr)
	{
		Inventory = Cast<UCInventory>(CreateWidget<UUserWidget>(GetWorld(), InventoryClass)); //�κ��丮 ���� �� �Ҵ�
		if (Inventory != nullptr)
		{
			Inventory->AddToViewport();
			Inventory->SetVisibility(ESlateVisibility::Hidden); //���� ������
		}
	}
}


// Called every frame
void UCInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCInventoryComponent::AddItem(FItemData ItemData)
{
	int index = FindStack(ItemData);
	Inventory->AddSlotToItem(ItemData);
	if (index == -1)
	{
		FSlotData newSlot;
		newSlot.ItemData = ItemData;
		newSlot.Amount = 1;
		SlotDatas.Add(newSlot); //���Կ� �߰�	
	}
	else
	{
		SlotDatas[index].Amount++; //���� ����
	}
}

void UCInventoryComponent::ShowInventory()
{
	if (!IsValid(Inventory)) return;
	if (!isInventory)
	{
		Inventory->SetVisibility(ESlateVisibility::Visible); 
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PlayerController->SetShowMouseCursor(true); 
		FInputModeGameAndUI InputMode; 
		InputMode.SetWidgetToFocus(Inventory->TakeWidget()); 
		InputMode.SetHideCursorDuringCapture(false); 
		PlayerController->SetInputMode(FInputModeGameAndUI(InputMode)); 
		isInventory = true;
	}
	else
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PlayerController->SetShowMouseCursor(false); 
		PlayerController->SetInputMode(FInputModeGameOnly()); 
		Inventory->SetVisibility(ESlateVisibility::Hidden); 
		isInventory = false;
	}
}

void UCInventoryComponent::DisplaySlotList()
{
	for (int i = 0; i < SlotDatas.Num(); i++)
	{
		const FString MessageB = FString::Printf(TEXT("Acquire Item Count :  %d"), SlotDatas.Num());

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, *MessageB);

		TArray<FStringFormatArg> args;
		args.Add(FStringFormatArg((SlotDatas[i]).ItemData.ItemName));
		args.Add(FStringFormatArg((SlotDatas[i]).ItemData.Price));
		args.Add(FStringFormatArg((SlotDatas[i]).ItemData.Description));
		args.Add(FStringFormatArg((SlotDatas[i]).Amount));

		const FString Message = FString::Format(TEXT("Name = {0}, value = {1}, Description = {2}, Amount = {3}"), args);

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, *Message);
	}
}

TArray<FSlotData> UCInventoryComponent::GetSlotDatas()
{
	return SlotDatas;
}

int UCInventoryComponent::FindStack(FItemData ItemData)
{
	//���� ��üũ�⸸ŭ �˻�
	for (int i = 0; i < SlotDatas.Num(); i++)
	{
		//���Կ� ���� �������� �ִٸ�
		if (SlotDatas[i].ItemData == ItemData)
		{
			return i; //�����ε��� ��ȯ
		}
	}
	return -1; //���ٸ� -1 ��ȯ
}

