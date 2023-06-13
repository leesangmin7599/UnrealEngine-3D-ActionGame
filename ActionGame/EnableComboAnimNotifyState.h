// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "EnableComboAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UEnableComboAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	//��Ƽ���� ���۽� ȣ��Ǵ� �Լ�
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	//��Ƽ���� �������� ȣ��Ǵ� �Լ�
};
