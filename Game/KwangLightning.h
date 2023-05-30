// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KwangLightning.generated.h"

UCLASS()
class GAME_API AKwangLightning : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKwangLightning();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Scene")
		class USceneComponent* DefaultRoot = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Capsule")
		class UCapsuleComponent* Capsule;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "LightningParticle")
		class UParticleSystemComponent* LightningParticle;
	
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
		class AKwangController* KwangController;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
		class AKwang* KwangPlayer;

	void AttackCheck();
	
	UFUNCTION()
		virtual void CapsuleOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// 충돌 끝났을때의 함수
	UFUNCTION()
		virtual void CapsuleOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
