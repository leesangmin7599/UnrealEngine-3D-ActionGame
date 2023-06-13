// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ActionGameGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AActionGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AActionGameGameModeBase();
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, category = "Widget")
		TSubclassOf<class UUserWidget> HealthWidget;

};

