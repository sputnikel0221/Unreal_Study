// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Untitled.h"
#include "AIController.h"
#include "NewAIController.generated.h"

UCLASS()
class UNTITLED_API ANewAIController : public AAIController
{
	GENERATED_BODY()


public:
	ANewAIController();
	virtual void OnPossess(APawn* InPawn) override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

private:
	UPROPERTY()
		class UBehaviorTree* BTAsset;
	UPROPERTY()
		class UBlackboardData* BBAsset;

public:
	void RunAI();
	void StopAI();
};

/* 이전요소들
pubic:
	virtual void OnUnPossess() override;

private:
	void OnRepeatTimer();

	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;
*/
