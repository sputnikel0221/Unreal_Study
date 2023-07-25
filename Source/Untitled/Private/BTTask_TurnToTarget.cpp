// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "NewCharacter.h"
#include "NewAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto NewCharacter = Cast<ANewCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == NewCharacter) return EBTNodeResult::Failed;

	auto Target = Cast<ANewCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANewAIController::TargetKey));
	if(nullptr == Target) return EBTNodeResult::Failed;

	// 1. 가져온 NPC와 Character(Target)위치를 바탕으로 Vector 생성
	FVector LookVector = Target->GetActorLocation() - NewCharacter->GetActorLocation();
	LookVector.Z = 0.0f;

	// 2. Vector를 바탕으로 Rotation 생성 / 적용
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	
	NewCharacter->SetActorRotation(
		FMath::RInterpTo(NewCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}
