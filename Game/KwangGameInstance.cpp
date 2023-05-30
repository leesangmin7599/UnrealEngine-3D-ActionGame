// Fill out your copyright notice in the Description page of Project Settings.


#include "KwangGameInstance.h"

UKwangGameInstance::UKwangGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Kwang(TEXT("DataTable'/Game/Data/FwangData.FwangData'"));
	if (DT_Kwang.Succeeded())
	{
		KwangTable = DT_Kwang.Object;
	}

}

void UKwangGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("DropExp of Level 1 Fwang %d"), GetKwangData(1)->DropExp);
}

FKwangData* UKwangGameInstance::GetKwangData(int32 level)
{
	return KwangTable->FindRow<FKwangData>(*FString::FromInt(level), TEXT(""));
}



