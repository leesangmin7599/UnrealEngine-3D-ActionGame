// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "EnemyAnimInstance.h"

UEnemyAnimInstance::UEnemyAnimInstance()
{
	speed = 0.f;
	direction = 0.f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackAsset(TEXT("AnimMontage'/Game/Blueprints/Enemy/Primary_Melee_A_Slow_Montage.Primary_Melee_A_Slow_Montage'"));
	if (AttackAsset.Succeeded())
	{
		AttackMontage = AttackAsset.Object;
	}
}

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (TryGetPawnOwner() == nullptr) return;
	MyEnemy = Cast<AEnemyCharacter>(TryGetPawnOwner());
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!IsValid(MyEnemy)) return;
	speed = MyEnemy->GetVelocity().Size2D();
	direction = CalculateDirection(MyEnemy->GetVelocity(), MyEnemy->GetControlRotation());
}

void UEnemyAnimInstance::PlayAttackAnim()
{
	Montage_Play(AttackMontage, 1.2f);
}

void UEnemyAnimInstance::AnimNotify_EnemyAttack()
{
	MyEnemy->SendDamage();
}
