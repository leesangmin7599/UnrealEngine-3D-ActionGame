// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "NextAttackAnimNotify.h"

void UNextAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp == nullptr) return;
	APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (MyPlayer == nullptr) return;
	MyPlayer->NextAttack();
}
