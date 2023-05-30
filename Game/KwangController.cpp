// Fill out your copyright notice in the Description page of Project Settings.


#include "KwangController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kwang.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "KwangLightning.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


AKwangController::AKwangController()
{
	bShowMouseCursor = true;
	bClickLeftMouse = false;
	bClickQueButton = false;
	bClickWButton = false;

	/*static ConstructorHelpers::FObjectFinder<UBlueprint> Lightning(TEXT("Blueprint'/Game/Blueprints/Kwang/BP_KwangLightning.BP_KwangLightning'"));
	if (Lightning.Object)
	{
		KwangLightning = (UClass*)Lightning.Object->GeneratedClass;
	}*/

	
}



void AKwangController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("RightClick", IE_Pressed, this, &AKwangController::InputRightButtonPressed);
	InputComponent->BindAction("RightClick", IE_Released, this, &AKwangController::InputRightButtonReleased);

	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AKwangController::InputLeftButtonPressed);
	InputComponent->BindAction("LeftClick", IE_Released, this, &AKwangController::InputLeftButtonReleased);

	InputComponent->BindAction("Lightning", IE_Pressed, this, &AKwangController::InputQueButtonPressed);

	InputComponent->BindAction("charging", IE_Pressed, this, &AKwangController::InputWButtonPressed);
	


}

void AKwangController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bClickRightMouse)
	{
		MoveToMouseCursor();
	}

}



void AKwangController::InputRightButtonPressed()
{
	bClickRightMouse = true;



}

void AKwangController::InputRightButtonReleased()
{
	bClickRightMouse = false;

}

void AKwangController::InputLeftButtonPressed()
{
	bClickLeftMouse = true;

	KwangPlayer = Cast<AKwang>(GetPawn());
	if (KwangPlayer)
	{
		LookMouseCursor();
		StopMovement();
		KwangPlayer->Attack();

	}

}

void AKwangController::InputLeftButtonReleased()
{
	bClickLeftMouse = false;




}

void AKwangController::SetNewDestination(const FVector Destination)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(Destination, MyPawn->GetActorLocation());
		if (Distance > 120.0f)
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, Destination);
		}
	}

}

void AKwangController::MoveToMouseCursor()
{
	KwangPlayer = Cast<AKwang>(GetPawn());
	if (!KwangPlayer->bIsAttacking && !KwangPlayer->bIsLightningAttacking && !KwangPlayer->bIschargingAttack && !KwangPlayer->bIsHitMotions)
	{
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			SetNewDestination(Hit.ImpactPoint);
		}
	}

}

void AKwangController::LookMouseCursor()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		APawn* const MyPawn = GetPawn();
		if (MyPawn)
		{
			FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(MyPawn->GetActorLocation(),
				FVector(Hit.Location.X, Hit.Location.Y, MyPawn->GetActorLocation().Z));
			MyPawn->SetActorRotation(LookRotation);
		}
	}
}

void AKwangController::InputQueButtonPressed()
{
	bClickQueButton = true;
	KwangPlayer = Cast<AKwang>(GetPawn());
	if (KwangPlayer == nullptr) return;

	if (!KwangPlayer->bIsLightningAttacking)
	{
		StopMovement();
		KwangPlayer->LightningAttack();
		LookMouseCursor();
	}
	
	
}

void AKwangController::InputWButtonPressed()
{
	KwangPlayer = Cast<AKwang>(GetPawn());
	if (KwangPlayer == nullptr) return;

	if (!KwangPlayer->bIschargingAttack)
	{
		StopMovement();
		KwangPlayer->chargingAttack();
		LookMouseCursor();
	}
	
}







