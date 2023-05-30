// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kwang.generated.h"

UCLASS()
class GAME_API AKwang : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKwang();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	 
	// 공격했을때 사용하는 함수 및 변수
	UPROPERTY(BlueprintReadOnly, VisibleAnyWhere, Category = "Attack")
	bool bIsAttacking; // 공격중인가
	UFUNCTION(BlueprintCallable)
	void Attack(); // 공격했을시
	UFUNCTION(BlueprintCallable)
	void AttackEnd(); // 공격이 끝났을시

	UPROPERTY(BlueprintReadOnly, VisibleAnyWhere, Category = "Lightning")
		bool bIsLightningAttacking; // 공격중인가
	UFUNCTION(BlueprintCallable)
		void LightningAttack(); // 공격했을시
	UFUNCTION(BlueprintCallable)
		void LightningAttackEnd(); // 공격이 끝났을시

	UPROPERTY(BlueprintReadOnly, VisibleAnyWhere, Category = "charging")
		bool bIschargingAttack; // 공격중인가
	UFUNCTION(BlueprintCallable)
		void chargingAttack(); // 공격했을시
	UFUNCTION(BlueprintCallable)
		void chargingAttackEnd(); // 공격이 끝났을시

	UPROPERTY(BlueprintReadOnly, VisibleAnyWhere, Category = "charging")
		bool bIsHitMotions;

	UFUNCTION(BlueprintCallable)
	void HitMotion();
	void AttackCheck();
	void LightningCheck();
	void chargingCheck();
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Anims")
		class UAnimMontage* CombatMontage; // montageblueprint에 넣어줄것
	int32 comboCount = 0; //콤보 카운트(4개의 연속공격을 사용해주기위해 만듬)

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Sound")
		class USoundCue* Swingsound;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Sound")
		class USoundCue* chargingSound;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Sound")
		class USoundCue* chargingAttackSound;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Sound")
		class USoundCue* LightningSound;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Sound")
		class USoundCue* HitSound;

	UFUNCTION(BlueprintCallable)
		void PlaySwingSound();
	UFUNCTION(BlueprintCallable)
		void PlaychargingSound();
	UFUNCTION(BlueprintCallable)
		void PlaychargingAttackSound();
	UFUNCTION(BlueprintCallable)
		void PlayLightningSound();

	// 공격시 적과 충돌할때 필요한 함수 및 변수
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Weapon")
		class USkeletalMeshComponent* WeaponSkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TSubclassOf<UDamageType> DamageTypeClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item | Combat")
		float Damage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		AController* WeaponInstigator;
	

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
		class AKwangController* KwangController;
	/*UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
		bool bLeftMouse;*/
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	UPROPERTY(VisibleAnywhere, Category = "Stat")
		class UKwangStatComponent* KwangStat;
	UPROPERTY(VisibleAnyWhere, Category = UI)
		class UWidgetComponent* HPBarWidget;
	


	FTimerHandle DeathTimer;
	FTimerHandle LightningTimers;
	FTimerHandle HitTimers;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
		float DeathDelay;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
		float LightningTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
		float HitTime;

	UFUNCTION(BlueprintCallable)
		void Die();
	UFUNCTION(BlueprintCallable)
		void DeathEnd();
	UFUNCTION(BlueprintCallable)
		void Disappear();

	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* CursorToWorld;

	UFUNCTION(BlueprintCallable)
		void LightningTimer();
	UFUNCTION(BlueprintCallable)
		void HitTimer();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Actor")
		TSubclassOf<class AActor> KwangLightning;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mana")
	float MaxMana;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mana")
	float Mana;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "CoolDown")
		float LightningSkillCoolDown = 1.0f;

	void LightningSkillCoolDownPlus();

	FTimerHandle LightningSkillCoolDowns;
	float SkillTimes = 1.0f;

private:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = true))
		class UCameraComponent* CameraComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = true))
		class USpringArmComponent* SpringArmComponent;
};
