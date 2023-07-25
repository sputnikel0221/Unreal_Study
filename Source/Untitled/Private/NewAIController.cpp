// Fill out your copyright notice in the Description page of Project Settings.


#include "NewAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

// Blackboard�� ���Ŭ������ �߰� �Ű�ü
const FName ANewAIController::HomePosKey(TEXT("HomePos"));
const FName ANewAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ANewAIController::TargetKey(TEXT("Target"));

ANewAIController::ANewAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Script/AIModule.BlackboardData'/Game/Book/AI/BB_NewBlackboardData.BB_NewBlackboardData'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Script/AIModule.BehaviorTree'/Game/Book/AI/BT_NewBehaviorTree.BT_NewBehaviorTree'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}


void ANewAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}


void ANewAIController::RunAI()
{
	UBlackboardComponent* TmpBB = Cast<UBlackboardComponent>(Blackboard); //UE5���� �߰�

	// BBAsset�� �����ϴ°�? + Blackboard�� ����Ѵ�
	if (UseBlackboard(BBAsset, TmpBB))
	{
		// Blackboard�� Vector�� ����, �����̸����� ������ ���� (FNAME�� �ʿ��� ����)
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			YU_LOG_FORMAT(Warning, TEXT("AI controller Coudlnt run BT"));
		}
	}

}
void ANewAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}

}



