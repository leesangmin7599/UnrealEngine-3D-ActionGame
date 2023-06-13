// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "EnableComboAnimNotifyState.h"

void UEnableComboAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{ //NotifyΩ√¿€
	if (MeshComp == nullptr) return;
	APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (MyPlayer == nullptr) return;
	MyPlayer->SetIsEnableCombo(true);
}

void UEnableComboAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{ //Notify≥°
	if (MeshComp == nullptr) return;
	APlayerCharacter* MyPlayer = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (MyPlayer == nullptr) return;
	MyPlayer->SetIsEnableCombo(false);
}
