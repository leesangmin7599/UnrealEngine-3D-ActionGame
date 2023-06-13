// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UHealthUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateHealthBar(float ratio);
private:
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* PB_HPBar;
};
