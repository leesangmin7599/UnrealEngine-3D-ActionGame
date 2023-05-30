// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KwangController.generated.h"

/**
 * 
 */
UCLASS()
class GAME_API AKwangController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AKwangController();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
		class AKwang* KwangPlayer;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actor")
		class AKwangLightning* KwangLightningParticle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actor")
		TSubclassOf<class AActor> KwangLightning;

	bool bClickRightMouse;
	bool bClickLeftMouse;
	bool bClickQueButton;
	bool bClickWButton;
	
public:

	UFUNCTION()
	void InputRightButtonPressed();
	UFUNCTION()
	void InputRightButtonReleased();
	UFUNCTION()
	void InputLeftButtonPressed();
	UFUNCTION()
	void InputLeftButtonReleased();
	UFUNCTION()
	void SetNewDestination(const FVector Destination);
	UFUNCTION()
	void MoveToMouseCursor();
	UFUNCTION()
	void LookMouseCursor();
	UFUNCTION()
	void InputQueButtonPressed();
	UFUNCTION()
		void InputWButtonPressed();
	

	

	virtual void SetupInputComponent() override;

	virtual void PlayerTick(float DeltaTime) override;


	



};
