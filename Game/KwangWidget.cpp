// Fill out your copyright notice in the Description page of Project Settings.


#include "KwangWidget.h"
#include "KwangStatComponent.h"
#include "Components/ProgressBar.h"

void UKwangWidget::BindKwangStat(class UKwangStatComponent* KwangStat)
{
	if (nullptr != KwangStat)
	{
		CurrentKwangStat = KwangStat;
		KwangStat->OnHPChanged.AddUObject(this, &UKwangWidget::UpdateHPWidget);
		
	}
}

void UKwangWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar2")));
	UpdateHPWidget();
}

void UKwangWidget::UpdateHPWidget()
{
	if (CurrentKwangStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentKwangStat->GetHPRatio());
		}
	}
}
