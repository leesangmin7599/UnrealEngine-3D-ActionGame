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

	//몽타주
	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* AttackMontage;
	UPROPERTY(VisibleAnywhere, Category = "Montage")
		class UAnimMontage* LastAttackMontage;
private: //공격
	bool isAttack = false; //공격 중인지
	bool isNextAttack = false; //다음 공격을 나갈 것인지
	bool isEnableCombo = false; //다음공격을 할 수 있는지
	int32 ComboCount = 0; //콤보 카운드
	bool isMove = true; //이동할 수 있는지
public:
	void Zoom(float value);
	void Attack(); //공격
	void NextAttack(); //다음공격
	void EndAttack(); //공격 종료
public:
	FORCEINLINE bool GetIsAttack() { return isAttack; }
	FORCEINLINE void SetIsAttack(bool value) { isAttack = value; }
	FORCEINLINE bool GetIsEnableCombo() { return isEnableCombo; }
	FORCEINLINE void SetIsEnableCombo(bool value) { isEnableCombo = value; }
	FORCEINLINE bool GetIsMove() { return isMove; }

private:
	float damage = 10.f; //공격력
	float currentHp = 100.f; //현재체력
	float maxHp = 100.f; //최대체력
	float attackRange = 150.f; //공격길이
	float attackRadius = 20.f; //공격두께
public:
	void SendDamage(); //데미지 전달 함수
protected:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override; //AnyDamage의 C++버전
	//ApplyDamage를 통해 들어온 데미지를 전달받는다.
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
