// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#include "HealthWidget.h"

void UHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();
	MyPlayer = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	MyPlayer->HealthEvent.BindUObject(this, &UHealthWidget::SetHealth); //��������Ʈ�� �Լ� ����
}

void UHealthWidget::SetHealth()
{
	HealthBar->SetPercent(MyPlayer->GetCurrentHp() / MyPlayer->GetMaxHp());

	FNumberFormattingOptions opts;
	//FNumberFormattingOptions : �Ҽ������� �ڸ����� ������ �����ϴ� ����ü

	opts.SetMaximumFractionalDigits(0);
	//0�� �־ �Ҽ����� ǥ������ ����

	CurrentHealthText->SetText(FText::AsNumber(MyPlayer->GetCurrentHp(), &opts));
	MaxHealthText->SetText(FText::AsNumber(MyPlayer->GetMaxHp(), &opts));
}
