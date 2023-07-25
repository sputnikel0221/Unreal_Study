// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "NewAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos"); // TaskNode�� �̸� ���� ����
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//1. Pawn�������� - Failed
	//OwnerComp��� Behavior Tree�� ������ AI ��Ʈ�ѷ��� ���� ���� ���� Pawn�� �������� ���
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	//2. NavigationSystem �������� - Failed
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
	return EBTNodeResult::Failed;

	//3. Orign(����)�� �������� (ref. HomePosKey)
	FVector Orign = OwnerComp.GetBlackboardComponent()->GetValueAsVector(ANewAIController::HomePosKey);

	//4. GetRandomPointInNavigableRadius ���� (ref. PatrolPosKey) 
	// SetValue�� ���� PatrolPosKey�� ���� - Succeeded
	FNavLocation NextPatrol;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ANewAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
