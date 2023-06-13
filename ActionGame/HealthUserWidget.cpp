// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/ProgressBar.h"

#include "HealthUserWidget.h"

void UHealthUserWidget::UpdateHealthBar(float ratio)
{
	if (PB_HPBar == nullptr) return;
	PB_HPBar->SetPercent(ratio);
}