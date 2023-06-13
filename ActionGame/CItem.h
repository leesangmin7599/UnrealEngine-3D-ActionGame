// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemData.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CItem.generated.h"

UCLASS()
class ACTIONGAME_API ACItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	virtual void OnConstruction(const FTransform& Transform) override;
private:
	UPROPERTY(EditAnywhere, Category = "ItemData")
		class UDataTable* ItemDataTable;
	UPROPERTY(EditAnywhere, Category = "ItemData")
		struct FDataTableRowHandle ItemRowHandle;
	UPROPERTY(VisibleAnywhere, Category = "ItemData")
		struct FItemData ItemData;
	UPROPERTY(VisibleAnywhere, Category = "ItemData")
		class USkeletalMeshComponent* ItemMesh;
	UPROPERTY(VisibleAnywhere, Category = "ItemData")
		class USphereComponent* SphereCollision;
protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	void SetItemRowName(FName itemName);
};
