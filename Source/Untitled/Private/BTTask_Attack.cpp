// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "NewAIController.h"
#include "NewCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// Character�� Attack �� Delegate�������� ����
	auto NewCharacter = Cast<ANewCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if(nullptr == NewCharacter) return EBTNodeResult::Failed;

	// 1. ����
	NewCharacter->Attack();
	IsAttacking = true;

	// 2. Delegate�� ���� �����Լ� ���ε�
	NewCharacter->OnAttackEnd.AddLambda([this]()-> void {
		IsAttacking = false;
		});

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
}
