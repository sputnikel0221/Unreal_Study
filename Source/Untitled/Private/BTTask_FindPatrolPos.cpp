// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "NewAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos"); // TaskNode의 이름 변경 가능
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//1. Pawn가져오기 - Failed
	//OwnerComp라는 Behavior Tree를 소유한 AI 컨트롤러의 현재 조종 중인 Pawn을 가져오는 기능
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	//2. NavigationSystem 가져오기 - Failed
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
	return EBTNodeResult::Failed;

	//3. Orign(원점)값 가져오기 (ref. HomePosKey)
	FVector Orign = OwnerComp.GetBlackboardComponent()->GetValueAsVector(ANewAIController::HomePosKey);

	//4. GetRandomPointInNavigableRadius 실행 (ref. PatrolPosKey) 
	// SetValue로 최종 PatrolPosKey값 설정 - Succeeded
	FNavLocation NextPatrol;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ANewAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
