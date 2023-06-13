// Fill out your copyright notice in the Description page of Project Settings.

#include "CInventoryComponent.h"

#include "Engine/CanvasRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacter.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// * �޽�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/ParagonTerra/Characters/Heroes/Terra/Skins/GryphonKnight/Meshes/Terra_GryphonKnight.Terra_GryphonKnight'"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	AutoPossessPlayer = EAutoReceiveInput::Player0; //0���÷��̾�

	// * ������ ��
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 500.f;
	SpringArm->SetRelativeRotation(FRotator(-35.f, 0, 0));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = false;

	// * ī�޶�
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// * ��Ÿ ����
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	Tags.Add(FName(TEXT("Player"))); //�÷��̾� �±� �߰�

	// * �ִϸ��̼�
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("AnimBlueprint'/Game/Blueprints/Player/ABP_Player.ABP_Player_C'"));
	if (AnimAsset.Succeeded())
	{
		GetMesh()->AnimClass = AnimAsset.Class;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMtgAsset(TEXT("AnimMontage'/Game/Blueprints/Player/PlayerAttackMontage.PlayerAttackMontage'"));
	if (AttackMtgAsset.Succeeded())
	{
		AttackMontage = AttackMtgAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> LastAttackMtgAsset(TEXT("AnimMontage'/Game/Blueprints/Player/PlayerLastAttackMontage.PlayerLastAttackMontage'"));
	if (LastAttackMtgAsset.Succeeded())
	{
		LastAttackMontage = LastAttackMtgAsset.Object;
	}

	// * �ʻ�ȭ
	PortraitArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MinimapArm"));
	PortraitArm->SetupAttachment(GetCapsuleComponent());
	PortraitArm->SetRelativeLocation(FVector(0, 0, 65.f));
	PortraitArm->SetRelativeRotation(FRotator(180.f, 0, 180.f));
	PortraitArm->TargetArmLength = 60.f;
	PortraitArm->bDoCollisionTest = false;

	PortraitCamera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MinimapCamera"));
	PortraitCamera->SetupAttachment(PortraitArm);
	PortraitCamera->SetRelativeLocation(FVector(0, 0, 0));
	PortraitCamera->SetRelativeRotation(FRotator(0, 0, 0));


	static ConstructorHelpers::FObjectFinder<UCanvasRenderTarget2D> PortraitAsset(TEXT("CanvasRenderTarget2D'/Game/Blueprints/Player/CRT_Portait.CRT_Portait'"));
	if (PortraitAsset.Succeeded())
	{
		PortraitCamera->TextureTarget = PortraitAsset.Object;
	}

	InventoryComponent = CreateDefaultSubobject<UCInventoryComponent>(TEXT("InventoryComponent"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle timerHandle;
	//GetWorld()->GetTimerManager().SetTimer(timerHandle, FTimerDelegate::CreateLambda([&]() {GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Unreal!!")); }), 10.f, false);
	PortraitCamera->ShowOnlyActorComponents(this, true); //�ڽŸ� ��, �ڽĵ� ����
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	HealthEvent.Unbind(); //�Լ��� ���� ����
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacter::Zoom(float value)
{
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength - (value * 10), 100.f, 1200.f);
}

void APlayerCharacter::Attack()
{
	if (!isAttack) //�������� �ƴ϶��
	{
		PlayAnimMontage(AttackMontage, 1.5f, FName(TEXT("Attack1")));
		//1.5������� ���ݸ�Ÿ�� ���, ������ Attack1
		isAttack = true; //���� ��
		ComboCount++;
	}
	else if (isEnableCombo)
	{
		isNextAttack = true;
	}
}

void APlayerCharacter::NextAttack()
{
	if (isNextAttack)
	{
		if (ComboCount == 1)
		{
			PlayAnimMontage(AttackMontage, 1.5f, FName(TEXT("Attack2")));
			isNextAttack = false;
			ComboCount++;
		}
		else if (ComboCount == 2)
		{
			PlayAnimMontage(LastAttackMontage, 1.5f);
			isNextAttack = false; 
			isMove = false; //������ �� ����
			ComboCount = 0; //�޺�ī��Ʈ �ʱ�ȭ
		}
	}
}

void APlayerCharacter::EndAttack()
{
	isMove = true;
	isAttack = false;
	isNextAttack = false;
	ComboCount = 0;
}

void APlayerCharacter::SendDamage()
{
	FVector start, end, direction;
	start = GetActorLocation();
	direction = GetActorForwardVector();
	end = start + (direction * attackRange);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TEnumAsByte<EObjectTypeQuery> PawnType = 
		UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	ObjectTypes.Add(PawnType);
	TArray<AActor*> IgnoreActors; 
	IgnoreActors.Add(this);
	FHitResult hitResult; 

	if (UKismetSystemLibrary::SphereTraceSingleForObjects
	(GetWorld(), start, end, attackRadius, ObjectTypes, false, IgnoreActors, 
		EDrawDebugTrace::ForDuration, hitResult, true, FLinearColor::Red, FLinearColor::Green, 0.5f))
	{
		if (hitResult.Actor.Get()->ActorHasTag(FName(TEXT("Enemy"))))
		{
			UGameplayStatics::ApplyDamage(
				hitResult.Actor.Get(), damage, 
				GetController(), this, 
				UDamageType::StaticClass());
		}
	}
}

float APlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	currentHp -= Damage;
	if (HealthEvent.IsBound())
	{
		HealthEvent.Execute();
	}
	if (currentHp <= 0)
	{
		GetCharacterMovement()->StopMovementImmediately(); //�̵�����
		GetCharacterMovement()->DisableMovement(); //�̵� ��Ȱ��ȭ
		GetController()->UnPossess(); //��Ʈ�ѷ��� ���� ����
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Destroy(); //���� ����
	}
	return Damage;
}

