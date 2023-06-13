// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

DECLARE_DELEGATE(FHealthEvent);

UCLASS()
class ACTIONGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
		class UCameraComponent* Camera;

	//��Ÿ��
	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* AttackMontage;
	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* LastAttackMontage;
private: //����
	bool isAttack = false; //���� ������
	bool isNextAttack = false; //���� ������ ���� ������
	bool isEnableCombo = false; //���������� �� �� �ִ���
	int32 ComboCount = 0; //�޺� ī���
	bool isMove = true; //�̵��� �� �ִ���
public:
	void Zoom(float value);
	void Attack(); //����
	void NextAttack(); //��������
	void EndAttack(); //���� ����
public:
	FORCEINLINE bool GetIsAttack() { return isAttack; }
	FORCEINLINE void SetIsAttack(bool value) { isAttack = value; }
	FORCEINLINE bool GetIsEnableCombo() { return isEnableCombo; }
	FORCEINLINE void SetIsEnableCombo(bool value) { isEnableCombo = value; }
	FORCEINLINE bool GetIsMove() { return isMove; }

private:
	float damage = 10.f; //���ݷ�
	float currentHp = 100.f; //����ü��
	float maxHp = 100.f; //�ִ�ü��
	float attackRange = 150.f; //���ݱ���
	float attackRadius = 20.f; //���ݵβ�
public:
	void SendDamage(); //������ ���� �Լ�
protected:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override; //AnyDamage�� C++����
	//ApplyDamage�� ���� ���� �������� ���޹޴´�.
public:
	FORCEINLINE float GetCurrentHp()
	{
		return currentHp;
	}
	FORCEINLINE float GetMaxHp()
	{
		return maxHp;
	}
public:
	FHealthEvent HealthEvent;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portrait", meta = (AllowPrivateAccess = true))
		class USpringArmComponent* PortraitArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portrait", meta = (AllowPrivateAccess = true))
		class USceneCaptureComponent2D* PortraitCamera;
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
		class UCInventoryComponent* InventoryComponent;
};
