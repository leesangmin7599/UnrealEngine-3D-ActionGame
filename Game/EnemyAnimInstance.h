// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHit);

UCLASS()
class GAME_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UEnemyAnimInstance();
	virtual void NativeInitializeAnimation() override; // beginplay와 같은 함수

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void UpdateAnimationProperties();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement")
		float MovementSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement")
		class APawn* Pawn;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
		class AEnemy* Enemy;
private:
	UFUNCTION()
		void AnimNotify_AttackHit();
public:
	FOnAttackHit OnAttackHit;
};
