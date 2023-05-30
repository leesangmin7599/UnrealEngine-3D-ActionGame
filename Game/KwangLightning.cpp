// Fill out your copyright notice in the Description page of Project Settings.


#include "KwangLightning.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TimerManager.h"
#include "KwangController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"

// Sets default values
AKwangLightning::AKwangLightning()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(DefaultRoot);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Capsule->SetupAttachment(DefaultRoot);
	Capsule->InitCapsuleSize(10.f, 10.f);
	Capsule->SetRelativeLocation(FVector(0.f, 0.f, 20.f));

	LightningParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("particle"));
	LightningParticle->SetupAttachment(DefaultRoot);
	
	float Damage = 50.f;

}

// Called when the game starts or when spawned
void AKwangLightning::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("ActorCreate!!"));
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AKwangLightning::CapsuleOnOverlapBegin);
	Capsule->OnComponentEndOverlap.AddDynamic(this, &AKwangLightning::CapsuleOnOverlapEnd);
	
}

// Called every frame
void AKwangLightning::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKwangLightning::AttackCheck()
{
	
}

void AKwangLightning::CapsuleOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AKwangLightning::CapsuleOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/*Destroy();*/
}



