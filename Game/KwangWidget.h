// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KwangWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API UKwangWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void BindKwangStat(class UKwangStatComponent* KwangStat);

protected:

	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	TWeakObjectPtr<class UKwangStatComponent> CurrentKwangStat;

	UPROPERTY()
		class UProgressBar* HPProgressBar;
};
