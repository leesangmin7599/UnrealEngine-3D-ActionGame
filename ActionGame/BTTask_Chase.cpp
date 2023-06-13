// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackboardKeys.h"
#include "EnemyAIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTask_Chase.h"

UBTTask_Chase::UBTTask_Chase()
{
    NodeName = TEXT("Chase");
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    AEnemyAIController* EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
    if (EnemyController == nullptr) return EBTNodeResult::Failed;

    AActor* Target = Cast<AActor>(EnemyController->GetBlackboardComponent()->GetValueAsObject(BBKeys::target)); //블랙보드에 저장된 타겟을 가져옴
    if (Target == nullptr) return EBTNodeResult::Failed;
    UAIBlueprintHelperLibrary::SimpleMoveToActor(EnemyController, Target); //컨트롤러를 Target으로 이동

    return EBTNodeResult::InProgress; //진행중 반환
}

void UBTTask_Chase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
    AEnemyAIController* EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
    if (EnemyController == nullptr) return;
    AActor* Target = Cast<AActor>(EnemyController->GetBlackboardComponent()->GetValueAsObject(BBKeys::target));
    if (Target == nullptr) return;
    if (FVector::Dist2D(Target->GetActorLocation(), EnemyController->GetPawn()->GetActorLocation()) < 50.f)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); //행동종료 후 성공 반환
    }
}
