// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	//virtual void NativeOnInitialized() override; //위젯 생성시 호출되는 함수
	virtual void NativeConstruct() override; //AddToViewport할때마다 호출되는 함수
	//virtual void NativeDestruct() override; //RemoveFromViewport할때마다 호출되는 함수
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//틱함수
private:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		class UTextBlock* CurrentHealthText;
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		class UTextBlock* MaxHealthText;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Player", meta = (AllowPrivateAccess = true))
		class APlayerCharacter* MyPlayer;
public:
	void SetHealth();
};
