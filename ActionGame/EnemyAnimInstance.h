// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UEnemyAnimInstance();
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character", meta = (AllowPrivateAccess = true))
		float speed;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character", meta = (AllowPrivateAccess = true))
		float direction;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character", meta = (AllowPrivateAccess = true))
		class AEnemyCharacter* MyEnemy;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Character", meta = (AllowPrivateAccess = true))
		class UAnimMontage* AttackMontage;
public:
	void PlayAttackAnim();

private:
	UFUNCTION()
		void AnimNotify_EnemyAttack();
};
