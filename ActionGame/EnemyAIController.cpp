// Fill out your copyright notice in the Description page of Project Settings.
#include "Kismet/KismetMathLibrary.h"
#include "PlayerCharacter.h"
#include "BlackboardKeys.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "EnemyAIController.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true; //틱함수 사용 여부
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception")));
	//AIPerception컴포넌트 생성
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	//블랙보드 컴포넌트 생성

	ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAsset(TEXT("BehaviorTree'/Game/Blueprints/Enemy/BT_Enemy2.BT_Enemy2'"));
	if (BTAsset.Succeeded())
	{
		BTree = BTAsset.Object;
	}

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 500.f; //볼 수 있는 거리
	SightConfig->LoseSightRadius = 550.f; //본 목표물을 볼 수 있는 거리
	SightConfig->PeripheralVisionAngleDegrees = 90.f; //시야각도
	SightConfig->SetMaxAge(3.f); //볼 수 있는 시간
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; //중립 인지

	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	//감지정보를 시야로 등록
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	//감지우선순위를 시야로 등록
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
	//자극에 정보가 변경되었다면 OnTargetDetected함수 호출

}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BTree == nullptr) return;
	RunBehaviorTree(BTree);//비헤이비어트리 시작
	if (Blackboard == nullptr) return;
	Blackboard->InitializeBlackboard(*BTree->BlackboardAsset);
	//비헤이비어트리에 연결된 블랙보드를 블랙보드 컴포넌트에 넣어줌
}

void AEnemyAIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	if (auto const Player = Cast<APlayerCharacter>(Actor))
	{
		GetBlackboardComponent()->SetValueAsBool(BBKeys::isFindPlayer, Stimulus.WasSuccessfullySensed());
		TSubclassOf<UAISense> AISense;
		TArray<AActor*> Actors;
		GetPerceptionComponent()->GetCurrentlyPerceivedActors(AISense, Actors);
		//현재 감지된 액터를 배열로 반환
		for (auto Other : Actors)
		{
			if (auto target = Cast<APlayerCharacter>(Other))
			{
				GetBlackboardComponent()->SetValueAsObject(BBKeys::target, target);
			}
		}
	}
}
