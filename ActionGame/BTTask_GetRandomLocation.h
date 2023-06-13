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
	//ExecuteTask : ���(�ൿ)����� ����Ǵ� �Լ�
	// * EBTNodeResult
	//- ExecuteTask�Լ��� 1���� �ߴܰ� 3���� ���� ��ȯ
	//- Succeeded : ����(�ൿ�� �����ߴ�)
	//- Failed : ����(�ൿ�� �����ߴ�)
	//- InProgress : ������(�½�ũ�� �����ϰ� ������ ����� ���� ��ȯ ����)
	//- Aborted : �ߴ�(�½�ũ ������ �ߴܵǾ���. ����� �����߰� ���� ��ȯ�ߴ� �� �� ����.)

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Search", meta = (AllowPrivateAccess = true))
		float SearchRadius = 300.f; //�������� Ž�� ����
};
