// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStatComponent.h"
#include "KwangGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"

// Sets default values for this component's properties
UEnemyStatComponent::UEnemyStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	// ...
	
	Level = 2;
}


// Called when the game starts
void UEnemyStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UEnemyStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetNewLevel(Level);
}

void UEnemyStatComponent::SetNewLevel(int32 NewLevel)
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
			UE_LOG(LogTemp, Warning, TEXT("Level (%d) data doesn't exist"), NewLevel);
		}
	}
}

void UEnemyStatComponent::SetDamage(float NewDamage)
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

void UEnemyStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0;
		OnHPIsZero.Broadcast(); //Enemy. begincpp에 람다를 활용해서 die함수를 호출하였음
	}
}

float UEnemyStatComponent::GetAttack()
{
	return CurrentStatData->Attack;
}

float UEnemyStatComponent::GetHPRatio()
{
	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0 : (CurrentHP / CurrentStatData->MaxHP);
}



