// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/SphereComponent.h"
#include "PlayerCharacter.h"
#include "CInventoryComponent.h"

#include "CItem.h"

// Sets default values
ACItem::ACItem()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DataAsset(TEXT("DataTable'/Game/DataTable/ItemDataTable.ItemDataTable'"));
	if (DataAsset.Succeeded())
	{
		ItemDataTable = DataAsset.Object;
		ItemRowHandle.DataTable = DataAsset.Object;
		ItemRowHandle.RowName = TEXT("Bow");
	}

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetCollisionProfileName(FName("Item"));
	ItemMesh->SetRelativeScale3D(FVector(0.5f));
	ItemMesh->SetSimulatePhysics(true);

	Tags.Add(FName("Item"));

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->AttachToComponent(ItemMesh, FAttachmentTransformRules::KeepRelativeTransform);
	SphereCollision->InitSphereRadius(100.f);

	RootComponent = ItemMesh;
}

// Called when the game starts or when spawned
void ACItem::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ItemData = *ItemDataTable->FindRow<FItemData>(ItemRowHandle.RowName, "");
	//행을 찾음

	if (&ItemData != nullptr && ItemMesh != nullptr)
	{
		ItemMesh->SetSkeletalMesh(ItemData.ItemMesh);
	}
}


void ACItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (auto player = Cast<APlayerCharacter>(OtherActor))
	{
		if (player->InventoryComponent != nullptr)
		{
			player->InventoryComponent->AddItem(ItemData);
			Destroy(); //제거
		}
	}
}

void ACItem::SetItemRowName(FName itemName)
{
	ItemRowHandle.RowName = itemName;
}