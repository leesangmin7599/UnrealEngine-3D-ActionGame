// Fill out your copyright notice in the Description page of Project Settings.

#include "CInventoryComponent.h"
#include "PlayerCharacter.h"
#include "MyPlayerController.h"
#define CheckNull(p) { if(p == nullptr) return; }

void AMyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	CheckNull(InPawn);
	MyPlayer = Cast<APlayerCharacter>(InPawn);
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis(TEXT("MoveForward"), this, &AMyPlayerController::MoveForward);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &AMyPlayerController::MoveRight);
	InputComponent->BindAxis(TEXT("Zoom"), this, &AMyPlayerController::Zoom);
	InputComponent->BindAxis(TEXT("Turn"), this, &AMyPlayerController::Turn);
	InputComponent->BindAxis(TEXT("LookUp"), this, &AMyPlayerController::LookUp);
	InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMyPlayerController::PlayerJump);
	InputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &AMyPlayerController::PlayerAttack);
	InputComponent->BindAction(TEXT("Inventory"), IE_Pressed, this, &AMyPlayerController::ShowInventory);
	InputComponent->BindAction(TEXT("InventoryData"), IE_Pressed, this, &AMyPlayerController::DisplayInventoryData);
}

void AMyPlayerController::MoveForward(float value)
{
	if (value != 0.f && MyPlayer->GetIsMove())
	{
		MyPlayer->AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), value);
		//��Ʈ�ѷ��� ȸ���� ���� X�� �������� �̵�
	}
}

void AMyPlayerController::MoveRight(float value)
{
	if (value != 0.f && MyPlayer->GetIsMove())
	{
		MyPlayer->AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), value);
		//��Ʈ�ѷ��� ȸ���� ���� Y�� �������� �̵�
	}
}

void AMyPlayerController::Zoom(float value)
{
	MyPlayer->Zoom(value);
}

void AMyPlayerController::Turn(float value)
{
	AddYawInput(value); //Yaw������ ȸ��
}

void AMyPlayerController::LookUp(float value)
{
	AddPitchInput(value); //Pitch������ ȸ��
}

void AMyPlayerController::PlayerJump()
{
	MyPlayer->Jump();
}

void AMyPlayerController::PlayerAttack()
{
	MyPlayer->Attack();
}

void AMyPlayerController::ShowInventory()
{
	MyPlayer->InventoryComponent->ShowInventory();
}

void AMyPlayerController::DisplayInventoryData()
{
	MyPlayer->InventoryComponent->DisplaySlotList();
}
