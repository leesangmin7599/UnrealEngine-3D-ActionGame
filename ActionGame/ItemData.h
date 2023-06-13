// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h" //�����־�� �Ѵ�.

UENUM(BlueprintType, meta = (ToolTip = "ItemType"))
enum class EItemType : uint8
{
	None UMETA(DisplayName = "None"),
	Equipment UMETA(DisplayName = "Equipment"), //���
	Countable UMETA(DisplayName = "Countable"), //�Ҹ�
	Etc UMETA(DisplayName = "Etc") //��Ÿ
};

USTRUCT(Atomic, BlueprintType)
struct FItemData : public FTableRowBase
{

	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemData")
		FString ItemName = FString(TEXT("")); 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemData")
		EItemType Type = EItemType::None; 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemData")
		int32 Price = 0; 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemData")
		FString Description = FString(TEXT(""));
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemData")
		class USkeletalMesh* ItemMesh = nullptr; 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ItemData")
		class UTexture2D* Icon = nullptr; 
	bool operator==(const FItemData& itemData)const
	{
		bool isName = (ItemName == itemData.ItemName);
		bool isType = (Type == itemData.Type);
		bool isPrice = (Price == itemData.Price);
		bool isMesh = (ItemMesh == itemData.ItemMesh);
		bool isIcon = (Icon == itemData.Icon);
		return (isName && isType && isPrice && isMesh && isIcon);
	}
};

USTRUCT(Atomic, BlueprintType)
struct FSlotData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlotData")
		FItemData ItemData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlotData")
		int32 Amount = 0;
	//mate = (MakeStructureValue = ��0��) �� ���� �⺻���� ���� �� �ִ�.
};