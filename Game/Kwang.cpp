// Fill out your copyright notice in the Description page of Project Settings.


#include "Kwang.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "KwangController.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "KwangAnimInstance.h"
#include "KwangStatComponent.h"
#include "Components/WidgetComponent.h"
#include "KwangWidget.h"
#include "Materials/Material.h"
#include "GameFramework/PlayerController.h"
#include "Components/DecalComponent.h"
#include "TimerManager.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Sound/SoundCue.h"


// Sets default values
AKwang::AKwang()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 640.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->TargetArmLength = 800.0f;
	SpringArmComponent->SetRelativeRotation(FRotator(-60.f, 45.f, 0.f));
	SpringArmComponent->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	CameraComponent->bUsePawnControlRotation = false; // 카메라의 회전이 폰의회전에 영향을주지않음

	DeathDelay = 0.3f;
	LightningTime = 0.5f;
	HitTime = 0.7f;
	
	KwangStat = CreateDefaultSubobject<UKwangStatComponent>(TEXT("KWANGSTAT"));
	
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	HPBarWidget->SetupAttachment(GetMesh());
	HPBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> KW_HUD(TEXT("WidgetBlueprint'/Game/Blueprints/UI/WBP_KwangHpBar.WBP_KwangHpBar_C'"));
	if (KW_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(KW_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.f, 50.f));
	}

	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Blueprints/Kwang/Mertiral/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.f, 32.f, 32.f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	static ConstructorHelpers::FObjectFinder<UBlueprint> Lightning(TEXT("Blueprint'/Game/Blueprints/Kwang/BP_KwangLightning.BP_KwangLightning'"));
	if (Lightning.Object)
	{
		KwangLightning = (UClass*)Lightning.Object->GeneratedClass;
	}
	
	MaxMana = 150;
	Mana = 150;
}

// Called when the game starts or when spawned
void AKwang::BeginPlay()
{
	Super::BeginPlay();
	UKwangAnimInstance* AnimInstance = Cast<UKwangAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnAttackHit.AddUObject(this, &AKwang::AttackCheck);
		AnimInstance->OnAttackHitLightning.AddUObject(this, &AKwang::LightningCheck);
		AnimInstance->OnAttackHitcharging.AddUObject(this, &AKwang::chargingCheck);
	}
	
	KwangStat->OnHPIsZero.AddLambda([this]()->void {
		Die();
		SetActorEnableCollision(false);
		});

	auto CharacterWidget = Cast<UKwangWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindKwangStat(KwangStat);
	}
	
	

}

// Called every frame
void AKwang::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CursorToWorld != nullptr)
	{
		if (UWorld* World = GetWorld())
		{
			FHitResult HitResult;
			FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
			FVector StartLocation = CameraComponent->GetComponentLocation();
			FVector EndLocation = CameraComponent->GetComponentRotation().Vector() * 2000.0f;
			Params.AddIgnoredActor(this);
			World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
			FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
			CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
		}

		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
	float ManaDrainRate = 1.f;
	float DeltaMana = ManaDrainRate * DeltaTime;
	if (Mana <= 150)
	{
		Mana += DeltaMana;
		//UE_LOG(LogTemp, Warning, TEXT("Mana : %f"), Mana);
	}
	
	
	
}

// Called to bind functionality to input
void AKwang::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AKwang::Attack()
{
	if (!bIsAttacking && !bIsLightningAttacking && !bIschargingAttack)
	{
		bIsAttacking = true;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && CombatMontage)
		{
			switch (comboCount)
			{
			case 0:
				PlayAnimMontage(CombatMontage, 1.0f, FName(TEXT("AttackA")));
				comboCount++;
				break;
			case 1:
				PlayAnimMontage(CombatMontage, 1.2f, FName(TEXT("AttackB")));
				comboCount++;
				break;
			case 2:
				PlayAnimMontage(CombatMontage, 1.f, FName(TEXT("AttackC")));
				comboCount++;
				break;
			case 3:
				PlayAnimMontage(CombatMontage, 1.5f, FName(TEXT("AttackD")));
				comboCount = 0;
				break;
			}
		}
	}	
}

void AKwang::AttackEnd()
{
	bIsAttacking = false;
	KwangController = Cast<AKwangController>(GetController());
	if (KwangController && KwangController->bClickLeftMouse)
	{
		Attack();
	}

}

void AKwang::LightningAttack()
{
	if (LightningSkillCoolDown >= 1)
	{
		if (Mana >= 50)
		{
			LightningSkillCoolDown = 0.0f;
			if (LightningSkillCoolDown <= 1)
			{
				GetWorldTimerManager().SetTimer(LightningSkillCoolDowns, this, &AKwang::LightningSkillCoolDownPlus, SkillTimes, true);
				UE_LOG(LogTemp, Warning, TEXT("Times : %f"), LightningSkillCoolDown);
			}
			UE_LOG(LogTemp, Warning, TEXT("Mana : %f"), Mana);
			if (!bIsLightningAttacking && !bIsAttacking && !bIschargingAttack)
			{
				bIsLightningAttacking = true;
				UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				Mana -= 40;
				if (AnimInstance && CombatMontage)
				{
					PlayAnimMontage(CombatMontage, 1.0f, FName(TEXT("Lightning")));
				}
				KwangController = Cast<AKwangController>(GetController());
				if (KwangController)
				{
					GetWorldTimerManager().SetTimer(LightningTimers, this, &AKwang::LightningTimer, LightningTime);
				}
			}
		}
	}
}

void AKwang::LightningSkillCoolDownPlus()
{
	LightningSkillCoolDown += 0.1f;
}

void AKwang::LightningTimer()
{
	FHitResult Hit;
	KwangController->GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	UE_LOG(LogTemp, Warning, TEXT("LightningAttackCreate"));
	FActorSpawnParameters SpawnParams;
	FRotator rotator;
	GetWorld()->SpawnActor<AActor>(KwangLightning, Hit.Location, rotator, SpawnParams);
}

void AKwang::LightningAttackEnd()
{
	bIsLightningAttacking = false;
}

void AKwang::chargingAttack()
{
	if (Mana >= 25)
	{
		if (!bIschargingAttack && !bIsAttacking && !bIsLightningAttacking)
		{
			bIschargingAttack = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			Mana -= 25;
			if (AnimInstance && CombatMontage)
			{
				PlayAnimMontage(CombatMontage, 1.0f, FName(TEXT("charging")));
			}
		}
	}
	
}

void AKwang::chargingAttackEnd()
{
	bIschargingAttack = false;
}



void AKwang::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 100.f;
	float AttackRadius = 50.f;
	TArray<FHitResult> HitReuslts;
	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	FVector Vec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + Vec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor;
	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	//DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius,
		//Rotation, DrawColor, false, 2.f);

	if (bResult && HitResult.Actor.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.Actor->GetName());

		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(10, DamageEvent, GetController(), this);
		// TakeDamage 에 20의 값을넣어주었으나
		// Stat->GetAttack()의 함스를 넣어주어도 됨
		//GetWorld()->SweepMultiByChannel()
		
			
	}
}

void AKwang::LightningCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 100.f;
	float AttackRadius = 50.f;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	FVector Vec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + Vec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor;
	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	//DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius,
		//Rotation, DrawColor, false, 2.f);
	if (bResult && HitResult.Actor.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Lightning Actor : %s"), *HitResult.Actor->GetName());

		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(100, DamageEvent, KwangController, this);
	}
}

void AKwang::chargingCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 100.f;
	float AttackRadius = 50.f;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius), Params);
	
	FVector Vec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + Vec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor;
	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	//DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius,
		//Rotation, DrawColor, false, 2.f);
	if (bResult && HitResult.Actor.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("charging Actor : %s"), *HitResult.Actor->GetName());

		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(25, DamageEvent, KwangController, this);
	}
}



float AKwang::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage2 = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("Actor Name : %s Damage : %f"), *GetName(), FinalDamage2);
	KwangStat->SetDamage(FinalDamage2);
	UGameplayStatics::PlaySound2D(this, HitSound);

	if (KwangStat->CurrentHP > 1 && !bIsLightningAttacking && !bIschargingAttack)
	{
		bIsHitMotions = true;
		PlayAnimMontage(CombatMontage, 1.0f, FName(TEXT("Hit")));
		GetWorldTimerManager().SetTimer(HitTimers, this, &AKwang::HitTimer, HitTime);
	}
	return FinalDamage2;
}

void AKwang::HitMotion()
{
	bIsHitMotions = false;
}

void AKwang::HitTimer()
{
	bIsAttacking = false;
	bIschargingAttack = false;
	bIsLightningAttacking = false;
}



void AKwang::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		PlayAnimMontage(CombatMontage, 1.0f, FName(TEXT("DeathEnd")));
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsAttacking = false;
}

void AKwang::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bUseAttachParentBound = true;
	GetWorldTimerManager().SetTimer(DeathTimer, this, &AKwang::Disappear, DeathDelay);
}

void AKwang::Disappear()
{
	Destroy();
}

void AKwang::PlaySwingSound()
{
	UGameplayStatics::PlaySound2D(this, Swingsound);
}

void AKwang::PlaychargingSound()
{
	UGameplayStatics::PlaySound2D(this, chargingSound);
}

void AKwang::PlaychargingAttackSound()
{
	UGameplayStatics::PlaySound2D(this, chargingAttackSound);
}

void AKwang::PlayLightningSound()
{
	UGameplayStatics::PlaySound2D(this, LightningSound);
}










