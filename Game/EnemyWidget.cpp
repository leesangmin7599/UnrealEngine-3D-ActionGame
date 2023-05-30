// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWidget.h"
#include "EnemyStatComponent.h"
#include "Components/ProgressBar.h"

void UEnemyWidget::BindCharacterStat(class UEnemyStatComponent* EnemyStat)
{
	if (nullptr != EnemyStat)
	{
		CurrentEnemyStat = EnemyStat;
		EnemyStat->OnHPChanged.AddUObject(this, &UEnemyWidget::UpdateHPWidget);
	}
}

void UEnemyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	UpdateHPWidget();
}

void UEnemyWidget::UpdateHPWidget()
{
	if (CurrentEnemyStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentEnemyStat->GetHPRatio());
		}
	}
}
