// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GetRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UBTTask_GetRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_GetRandomLocation();
protected:
	EBTNodeResult::Type ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//ExecuteTask : 노드(행동)실행시 실행되는 함수
	// * EBTNodeResult
	//- ExecuteTask함수는 1개의 중단과 3개의 값을 반환
	//- Succeeded : 성공(행동에 성공했다)
	//- Failed : 실패(행동에 실패했다)
	//- InProgress : 진행중(태스크를 수행하고 있으며 결과는 추후 반환 예정)
	//- Aborted : 중단(태스크 실행중 중단되었다. 결과는 실패했고 값을 반환했다 할 수 없다.)

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Search", meta = (AllowPrivateAccess = true))
		float SearchRadius = 300.f; //랜덤지점 탐색 범위
};
