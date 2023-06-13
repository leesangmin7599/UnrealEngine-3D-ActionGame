// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void OnPossess(APawn* InPawn) override;
	//플레이어와 빙의시 호출되는 함수(있어야 빙의된다)
	virtual void SetupInputComponent() override;
	//입력받는 함수
private: //입력시 호출 될 함수
	UFUNCTION()
		void MoveForward(float value);
	UFUNCTION()
		void MoveRight(float value);
	UFUNCTION()
		void Zoom(float value);
	UFUNCTION()
		void Turn(float value);
	UFUNCTION()
		void LookUp(float value);
	UFUNCTION()
		void PlayerJump();
	UFUNCTION()
		void PlayerAttack();
	UFUNCTION()
		void ShowInventory();
	UFUNCTION()
		void DisplayInventoryData();
private:
	UPROPERTY()
		class APlayerCharacter* MyPlayer;
};
