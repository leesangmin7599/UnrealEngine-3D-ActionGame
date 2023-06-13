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
	// * 메쉬
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/ParagonTerra/Characters/Heroes/Terra/Skins/GryphonKnight/Meshes/Terra_GryphonKnight.Terra_GryphonKnight'"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	AutoPossessPlayer = EAutoReceiveInput::Player0; //0번플레이어

	// * 스프링 암
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 500.f;
	SpringArm->SetRelativeRotation(FRotator(-35.f, 0, 0));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = false;

	// * 카메라
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// * 기타 설정
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	Tags.Add(FName(TEXT("Player"))); //플레이어 태그 추가

	// * 애니메이션
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

	// * 초상화
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
	PortraitCamera->ShowOnlyActorComponents(this, true); //자신만 봄, 자식들 포함
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	HealthEvent.Unbind(); //함수와 연결 해제
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
	if (!isAttack) //공격중이 아니라면
	{
		PlayAnimMontage(AttackMontage, 1.5f, FName(TEXT("Attack1")));
		//1.5배속으로 공격몽타주 재생, 섹션은 Attack1
		isAttack = true; //공격 중
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
			isMove = false; //움직일 수 없음
			ComboCount = 0; //콤보카운트 초기화
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
		GetCharacterMovement()->StopMovementImmediately(); //이동중지
		GetCharacterMovement()->DisableMovement(); //이동 비활성화
		GetController()->UnPossess(); //컨트롤러와 연결 해제
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Destroy(); //액터 제거
	}
	return Damage;
}

