// Copyright Epic Games, Inc. All Rights Reserved.

#include "Blueprint/UserWidget.h"

#include "PlayerCharacter.h"
#include "MyPlayerController.h"
#include "ActionGameGameModeBase.h"

AActionGameGameModeBase::AActionGameGameModeBase()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetAsset(TEXT("WidgetBlueprint'/Game/Blueprints/Player/CBP_HealthWidget.CBP_HealthWidget_C'"));
	if(WidgetAsset.Class != nullptr)
	{
		HealthWidget = WidgetAsset.Class;
	}
}

void AActionGameGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	if(HealthWidget != nullptr)
	{
		CreateWidget<UUserWidget>(GetWorld(), HealthWidget)->AddToViewport();
	}
}
