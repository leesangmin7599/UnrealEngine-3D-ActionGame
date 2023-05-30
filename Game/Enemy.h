// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"


UCLASS()
class GAME_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
		float Health;

	FORCEINLINE float GetHealth() { return Health; }

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
		float MaxHealth = 100;

	FORCEINLINE float GetMaxHealth() { return MaxHealth; }

	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
		TSubclassOf<UDamageType> DamageTypeClass;

	// enemy에 달아준 component들 공격, 대기(따라가기)를 나타내기위함.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "AI")
		class USphereComponent* AgroSphere; // 닿으면 character에 다가올 sphere

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "AI")
		class USphereComponent* CombatSphere; // 닿으면 공격모션이 나올 sphere

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "AI")
		class AAIController* AIController;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AI")
		class UParticleSystem* HitParticles;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Sound")
		class USoundCue* HitSound;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "AI")
		bool bOverlappingCombatSphere;

	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
		class UAnimMontage* CombatMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
		bool bAttacking;

	void AttackCheck();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;

	FTimerHandle AttackTimer;
	FTimerHandle MoveTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float AttackMinTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float AttackMaxTime;

	UFUNCTION(BlueprintCallable)
		void MoveToTarget(class AKwang* Target);
	UFUNCTION(BlueprintCallable)
		void Attack();
	UFUNCTION(BlueprintCallable)
		void AttackEnd();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Die();


	FTimerHandle DeathTimer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
		float DeathDelay;

	UFUNCTION(BlueprintCallable)
		void DeathEnd();
	UFUNCTION(BlueprintCallable)
		void Disappear();

	UPROPERTY(VisibleAnywhere)
		class UEnemyStatComponent* Stat;
	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* HpBarWidget;

	

	// 충돌했을때 함수
	UFUNCTION()
		virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// 충돌 끝났을때의 함수
	UFUNCTION()
		virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 충돌했을때 함수 (이 구체에 충돌하면공격을 시행하기위해 만든함수)
	UFUNCTION()
		virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// 충돌 끝났을때의 함수 (이 구체에서 충돌이 끝나면 나오면 공격ㄱ을 해제)
	UFUNCTION()
		virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
