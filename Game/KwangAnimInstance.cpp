// Fill out your copyright notice in the Description page of Project Settings.


#include "KwangAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kwang.h"
#include "KwangController.h"

UKwangAnimInstance::UKwangAnimInstance()
{
	
}

void UKwangAnimInstance::UpdateAnimationProperties()
{
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Speed = Pawn->GetVelocity().Size();
		if (KwangPlayer == nullptr)
		{
			KwangPlayer = Cast<AKwang>(Pawn);
		}
	}
}

void UKwangAnimInstance::AnimNotify_AttackHit()
{
	OnAttackHit.Broadcast();
}

void UKwangAnimInstance::AnimNotify_LightningHit()
{
	OnAttackHitLightning.Broadcast();
}

void UKwangAnimInstance::AnimNotify_chargingHit()
{
	OnAttackHitcharging.Broadcast();
}
