// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "PlayerAnimInstance.h"

#define CheckNull(p) { if(p == nullptr) return; }

UPlayerAnimInstance::UPlayerAnimInstance()
{
	Speed = 0.f;
	Direction = 0.f;
	isAir = false;
}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	CheckNull(TryGetPawnOwner());
	MyPlayer = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!IsValid(MyPlayer)) return;
	Speed = MyPlayer->GetVelocity().Size2D();
	//높이를 빼기 위해 Size2D
	Direction = CalculateDirection(MyPlayer->GetVelocity(), MyPlayer->GetControlRotation());
	isAir = MyPlayer->GetMovementComponent()->IsFalling();
	//추락중인지 여부
}

void UPlayerAnimInstance::AnimNotify_Damage()
{
	if (!IsValid(MyPlayer))return;
	MyPlayer->SendDamage();
}
