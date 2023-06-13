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
	PrimaryActorTick.bCanEverTick = true; //ƽ�Լ� ��� ����
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception")));
	//AIPerception������Ʈ ����
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	//������ ������Ʈ ����

	ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAsset(TEXT("BehaviorTree'/Game/Blueprints/Enemy/BT_Enemy2.BT_Enemy2'"));
	if (BTAsset.Succeeded())
	{
		BTree = BTAsset.Object;
	}

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 500.f; //�� �� �ִ� �Ÿ�
	SightConfig->LoseSightRadius = 550.f; //�� ��ǥ���� �� �� �ִ� �Ÿ�
	SightConfig->PeripheralVisionAngleDegrees = 90.f; //�þ߰���
	SightConfig->SetMaxAge(3.f); //�� �� �ִ� �ð�
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; //�߸� ����

	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	//���������� �þ߷� ���
	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	//�����켱������ �þ߷� ���
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
	//�ڱؿ� ������ ����Ǿ��ٸ� OnTargetDetected�Լ� ȣ��

}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BTree == nullptr) return;
	RunBehaviorTree(BTree);//�����̺��Ʈ�� ����
	if (Blackboard == nullptr) return;
	Blackboard->InitializeBlackboard(*BTree->BlackboardAsset);
	//�����̺��Ʈ���� ����� �����带 ������ ������Ʈ�� �־���
}

void AEnemyAIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	if (auto const Player = Cast<APlayerCharacter>(Actor))
	{
		GetBlackboardComponent()->SetValueAsBool(BBKeys::isFindPlayer, Stimulus.WasSuccessfullySensed());
		TSubclassOf<UAISense> AISense;
		TArray<AActor*> Actors;
		GetPerceptionComponent()->GetCurrentlyPerceivedActors(AISense, Actors);
		//���� ������ ���͸� �迭�� ��ȯ
		for (auto Other : Actors)
		{
			if (auto target = Cast<APlayerCharacter>(Other))
			{
				GetBlackboardComponent()->SetValueAsObject(BBKeys::target, target);
			}
		}
	}
}
