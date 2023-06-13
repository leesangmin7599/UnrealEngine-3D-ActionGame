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
	//�÷��̾�� ���ǽ� ȣ��Ǵ� �Լ�(�־�� ���ǵȴ�)
	virtual void SetupInputComponent() override;
	//�Է¹޴� �Լ�
private: //�Է½� ȣ�� �� �Լ�
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
