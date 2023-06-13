// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthUserWidget.h"
#include "Components/WidgetComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "EnemyAIController.h"
#include "EnemyCharacter.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("SkeletalMesh'/Game/ParagonSunWukong/Characters/Heroes/Wukong/Skins/Infernal/Meshes/Wukong_Infernal.Wukong_Infernal'"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.f, 0));

	ConstructorHelpers::FClassFinder<UAnimInstance> AnimAsset(TEXT("AnimBlueprint'/Game/Blueprints/Enemy/ABP_Enemy.ABP_Enemy_c'"));
	if (AnimAsset.Succeeded())
	{
		GetMesh()->AnimClass = AnimAsset.Class;
	}

	AIControllerClass = AEnemyAIController::StaticClass();
	//자동으로 연결될 AIController
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//EAutoPossessAI : 생성시 연결될것인지, 레벨에 배치됐을때 연결될 것인지
	Tags.Add(FName(TEXT("Enemy"))); //태그 추가

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	static ConstructorHelpers::FClassFinder<UHealthUserWidget> HealthAsset(TEXT("WidgetBlueprint'/Game/Blueprints/Enemy/WB_HealthWidget.WB_HealthWidget_C'"));
	if (HealthAsset.Succeeded())
	{
		HPBarWidget->SetWidgetClass(HealthAsset.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.f, 50.f));
	}
	HPBarWidget->SetupAttachment(GetMesh());
	HPBarWidget->SetRelativeLocation(FVector(0, 0, 180));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	currentHp -= DamageAmount;

	UHealthUserWidget* HPBar = Cast<UHealthUserWidget>(HPBarWidget->GetUserWidgetObject());
	if (HPBar == nullptr) return DamageAmount;
	HPBar->UpdateHealthBar(GetRatio());

	if (currentHp <= 0)
	{
		Destroy();
	}

	return DamageAmount;
}

void AEnemyCharacter::SendDamage()
{
	FVector start, end, direction;
	start = GetActorLocation();
	direction = GetActorForwardVector();
	end = start + (direction * attackRange);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	//충돌가능 오브젝트 유형
	TEnumAsByte<EObjectTypeQuery> PawnType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn); //폰타입

	ObjectTypes.Add(PawnType);
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this); //무시할 액터 배열
	FHitResult hitResult;

	if (UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), start, end, attackRadius, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::ForDuration, hitResult, true, FLinearColor::Red, FLinearColor::Green, 0.5f))
	{
		if (hitResult.GetActor()->ActorHasTag(FName(TEXT("Player"))))
		{
			UGameplayStatics::ApplyDamage(hitResult.GetActor(), damage, GetController(), this, UDamageType::StaticClass());
		}
	}
}

