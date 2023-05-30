// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KwangAnimInstance.generated.h"
DECLARE_MULTICAST_DELEGATE(FOnAttackHit);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitLightning);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitcharging);
/**
 * 
 */
UCLASS()
class GAME_API UKwangAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UKwangAnimInstance();

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void UpdateAnimationProperties();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Movement")
		class AKwang* KwangPlayer;
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float Speed;

private:
	UFUNCTION()
		void AnimNotify_AttackHit();
	UFUNCTION()
		void AnimNotify_LightningHit();
	UFUNCTION()
		void AnimNotify_chargingHit();
public:
	FOnAttackHit OnAttackHit;
	FOnAttackHitLightning OnAttackHitLightning;
	FOnAttackHitcharging OnAttackHitcharging;

	
};
