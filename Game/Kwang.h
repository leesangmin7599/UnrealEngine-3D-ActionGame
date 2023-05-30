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
	 
	// ���������� ����ϴ� �Լ� �� ����
	UPROPERTY(BlueprintReadOnly, VisibleAnyWhere, Category = "Attack")
	bool bIsAttacking; // �������ΰ�
	UFUNCTION(BlueprintCallable)
	void Attack(); // ����������
	UFUNCTION(BlueprintCallable)
	void AttackEnd(); // ������ ��������

	UPROPERTY(BlueprintReadOnly, VisibleAnyWhere, Category = "Lightning")
		bool bIsLightningAttacking; // �������ΰ�
	UFUNCTION(BlueprintCallable)
		void LightningAttack(); // ����������
	UFUNCTION(BlueprintCallable)
		void LightningAttackEnd(); // ������ ��������

	UPROPERTY(BlueprintReadOnly, VisibleAnyWhere, Category = "charging")
		bool bIschargingAttack; // �������ΰ�
	UFUNCTION(BlueprintCallable)
		void chargingAttack(); // ����������
	UFUNCTION(BlueprintCallable)
		void chargingAttackEnd(); // ������ ��������

	UPROPERTY(BlueprintReadOnly, VisibleAnyWhere, Category = "charging")
		bool bIsHitMotions;

	UFUNCTION(BlueprintCallable)
	void HitMotion();
	void AttackCheck();
	void LightningCheck();
	void chargingCheck();
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Anims")
		class UAnimMontage* CombatMontage; // montageblueprint�� �־��ٰ�
	int32 comboCount = 0; //�޺� ī��Ʈ(4���� ���Ӱ����� ������ֱ����� ����)

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

	// ���ݽ� ���� �浹�Ҷ� �ʿ��� �Լ� �� ����
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
