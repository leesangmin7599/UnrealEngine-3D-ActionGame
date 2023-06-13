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
	MyPlayer->HealthEvent.BindUObject(this, &UHealthWidget::SetHealth); //델리게이트에 함수 연결
}

void UHealthWidget::SetHealth()
{
	HealthBar->SetPercent(MyPlayer->GetCurrentHp() / MyPlayer->GetMaxHp());

	FNumberFormattingOptions opts;
	//FNumberFormattingOptions : 소수점같은 자릿수의 포멧을 지정하는 구조체

	opts.SetMaximumFractionalDigits(0);
	//0을 넣어서 소수점은 표시하지 않음

	CurrentHealthText->SetText(FText::AsNumber(MyPlayer->GetCurrentHp(), &opts));
	MaxHealthText->SetText(FText::AsNumber(MyPlayer->GetMaxHp(), &opts));
}
