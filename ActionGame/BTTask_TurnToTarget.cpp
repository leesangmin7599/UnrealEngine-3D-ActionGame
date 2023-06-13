// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackboardKeys.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_TurnToTarget.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
    NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    auto Enemy = Cast<AEnemyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (Enemy == nullptr) return EBTNodeResult::Failed;

    auto Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKeys::target));
    if (Target == nullptr) return EBTNodeResult::Failed;

    FVector LookVector = Target->GetActorLocation() - Enemy->GetActorLocation();

    LookVector.Z = 0.f;
    FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();

    Enemy->SetActorRotation(FMath::RInterpTo(Enemy->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));


    return EBTNodeResult::Succeeded;
}
