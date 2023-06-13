// Fill out your copyright notice in the Description page of Project Settings.

#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"
#include "BlackboardKeys.h"
#include "NavigationSystem.h"

#include "BTTask_GetRandomLocation.h"

UBTTask_GetRandomLocation::UBTTask_GetRandomLocation()
{
    NodeName = TEXT("GetRandomLocation");
}

EBTNodeResult::Type UBTTask_GetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AEnemyAIController* EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
    if (EnemyController == nullptr) return EBTNodeResult::Failed; //실패 반환
    APawn* Enemy = EnemyController->GetPawn();
    if (Enemy == nullptr) return EBTNodeResult::Failed; //실패 반환
    FVector Origin = Enemy->GetActorLocation(); //에너미 위치
    FNavLocation NavPos; //네비게이션 기반 위치저장 구조체
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    //현재 월드에 있는 네비게이션 시스템을 가져옴
    if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, NavPos))
    {//네비게이션 기반 랜덤위치를 NavPos에 넣어줌
        EnemyController->GetBlackboardComponent()->SetValueAsVector(BBKeys::randomPos, NavPos.Location);
        //랜덤한 위치를 블랙보드의 randomPos에 저장

        return EBTNodeResult::Succeeded; //성공반환
    }
    return EBTNodeResult::Failed; //실패 반환
}
