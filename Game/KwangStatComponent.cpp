// Fill out your copyright notice in the Description page of Project Settings.


#include "KwangStatComponent.h"
#include "KwangGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UKwangStatComponent::UKwangStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	Level = 1;
}


// Called when the game starts
void UKwangStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UKwangStatComponent::InitializeComponent()
{
	SetNewLevel(Level);
}

void UKwangStatComponent::SetNewLevel(int32 NewLevel)
{
	auto KwangInstance = Cast<UKwangGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (KwangInstance != nullptr)
	{
		CurrentStatData = KwangInstance->GetKwangData(NewLevel);
		if (nullptr != CurrentStatData)
		{
			Level = NewLevel;
			SetHP(CurrentStatData->MaxHP);
			CurrentHP = CurrentStatData->MaxHP;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Level (%d) data doesn't exist"), NewLevel);
		}
	}
}

void UKwangStatComponent::SetDamage(float NewDamage)
{
	if (CurrentStatData != nullptr)
	{
		/*CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0, CurrentStatData->MaxHP);
		if (CurrentHP <= 0)
		{
			OnHPIsZero.Broadcast();
		}*/
		SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0, CurrentStatData->MaxHP));
	}
}

void UKwangStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0;
		OnHPIsZero.Broadcast();
	}
}

float UKwangStatComponent::GetAttack()
{
	return CurrentStatData->Attack;
}

float UKwangStatComponent::GetHPRatio()
{
	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0 : (CurrentHP / CurrentStatData->MaxHP);
}




