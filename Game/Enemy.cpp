// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kwang.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "EnemyWidget.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "EnemyStatComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyAnimInstance.h"
#include "Sound/SoundCue.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->InitSphereRadius(1200.f);

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(120.f);
	

	GetCharacterMovement()-> MaxWalkSpeed = 450.f;
	bAttacking = false;
	bOverlappingCombatSphere = false;

	AttackMinTime = 0.5f;
	AttackMaxTime = 4.0f;

	MaxHealth = 100;
	Health = 100;
	
	AttackRange = 200.f;
	AttackRadius = 50.f;
	
	
	DeathDelay = 0.3f;

	

	Stat = CreateDefaultSubobject<UEnemyStatComponent>(TEXT("STAT"));

	HpBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	HpBarWidget->SetupAttachment(GetMesh());
	HpBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	HpBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget>Hp_HUD(TEXT("WidgetBlueprint'/Game/Blueprints/UI/WBP_EnemyHpBar.WBP_EnemyHpBar_C'"));
	if (Hp_HUD.Succeeded())
	{
		HpBarWidget->SetWidgetClass(Hp_HUD.Class);
		HpBarWidget->SetDrawSize(FVector2D(150.f, 50.f));
	}

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());

	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);

	Stat->OnHPIsZero.AddLambda([this]()->void {
		Die();
		SetActorEnableCollision(false);
		});

	auto CharacterWidget = Cast<UEnemyWidget>(HpBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(Stat);
	}

	UEnemyAnimInstance* AnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnAttackHit.AddUObject(this, &AEnemy::AttackCheck);
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void AEnemy::MoveToTarget(AKwang* Target)
{


	if (AIController)
	{
		FAIMoveRequest MoveReauest;
		MoveReauest.SetGoalActor(Target);
		MoveReauest.SetAcceptanceRadius(5.0f);

		FNavPathSharedPtr NavPath;
		AIController->MoveTo(MoveReauest, &NavPath);
	}
}
void AEnemy::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AKwang* KwangPlayer = Cast<AKwang>(OtherActor);
		if (KwangPlayer)
		{
			MoveToTarget(KwangPlayer);
			
		}
	}
}

void AEnemy::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AKwang* KwangPlayer = Cast<AKwang>(OtherActor);
		if (KwangPlayer)
		{
			if (AIController)
			{
				AIController->StopMovement();
				
			}
		}
	}
}

void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AKwang* KwangPlayer = Cast<AKwang>(OtherActor);
		if (KwangPlayer)
		{
			bOverlappingCombatSphere = true;
			Attack();
			
		}
	}
}

void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AKwang* KwangPlayer = Cast<AKwang>(OtherActor);
		if (KwangPlayer)
		{
			bOverlappingCombatSphere = false;
			GetWorldTimerManager().ClearTimer(AttackTimer);
			MoveToTarget(KwangPlayer);
		
		}
	}
}

void AEnemy::Attack()
{
	if (!bAttacking)
	{
		if (AIController)
		{
			AIController->StopMovement();
		}
		bAttacking = true;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			PlayAnimMontage(CombatMontage, 1.0f, FName(TEXT("Attack")));
		}
	}
}

void AEnemy::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 200.f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(50.0f),
		Params);

	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius; // 반지름
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat(); // 캡슐 회전방향
	FColor DrawColor = bResult ? FColor::Green : FColor::Red; // 발견시 녹색, 미발견시 적색
	float DebugLifeTime = 2.0f; // 생성후 삭제시간
	//DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);

	if(bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(10, DamageEvent, GetController(), this);
			// TakeDamage 에 20의 값을넣어주었으나
			// Stat->GetAttack()의 함스를 넣어주어도 됨
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
		}
	}
}

void AEnemy::AttackEnd()
{
	bAttacking = false;
	if(bOverlappingCombatSphere)
	{
		float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
		
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("Actor Name : %s Damage : %f"), *GetName(), FinalDamage);
	Stat->SetDamage(FinalDamage);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, GetActorLocation(), FRotator(0.f), false);
	UGameplayStatics::PlaySound2D(this, HitSound);

	return FinalDamage;
	
	
	
}

void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		PlayAnimMontage(CombatMontage, 1.0f, FName(TEXT("Death")));
	}
	
	AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MoveToTarget(false);
	bAttacking = false;
}

void AEnemy::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bUseAttachParentBound = true;
	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disappear, DeathDelay);
	
}

void AEnemy::Disappear()
{
	Destroy();
}







