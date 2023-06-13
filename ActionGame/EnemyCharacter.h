// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class ACTIONGAME_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override; 
	//ApplyDamage함수로 데미지를 전달받는 함수

public:
	void SendDamage(); //데미지를 전달할 함수

private:
	UPROPERTY()
		float damage = 10.f;
	UPROPERTY()
		float currentHp = 100.f;
	UPROPERTY()
		float maxHp = 100.f;
	UPROPERTY()
		float attackRange = 150.f;
	UPROPERTY()
		float attackRadius = 20.f;

private:
	UPROPERTY()
		class UWidgetComponent* HPBarWidget;
public:
	FORCEINLINE float GetRatio()
	{
		return (currentHp > 0) ? (currentHp / maxHp) : 0.f;
	}


};
