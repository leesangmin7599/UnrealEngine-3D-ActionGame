// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "KwangGameInstance.generated.h"

USTRUCT()
struct FKwangData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FKwangData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30){}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 DropExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 NextExp;
};

/**
 * 
 */

UCLASS()
class GAME_API UKwangGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UKwangGameInstance();

	virtual void Init() override;

	FKwangData* GetKwangData(int32 level);
private:
	UPROPERTY()
		class UDataTable* KwangTable;
};
