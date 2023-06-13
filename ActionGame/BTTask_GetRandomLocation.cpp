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
    if (EnemyController == nullptr) return EBTNodeResult::Failed; //���� ��ȯ
    APawn* Enemy = EnemyController->GetPawn();
    if (Enemy == nullptr) return EBTNodeResult::Failed; //���� ��ȯ
    FVector Origin = Enemy->GetActorLocation(); //���ʹ� ��ġ
    FNavLocation NavPos; //�׺���̼� ��� ��ġ���� ����ü
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
    //���� ���忡 �ִ� �׺���̼� �ý����� ������
    if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, NavPos))
    {//�׺���̼� ��� ������ġ�� NavPos�� �־���
        EnemyController->GetBlackboardComponent()->SetValueAsVector(BBKeys::randomPos, NavPos.Location);
        //������ ��ġ�� �������� randomPos�� ����

        return EBTNodeResult::Succeeded; //������ȯ
    }
    return EBTNodeResult::Failed; //���� ��ȯ
}
