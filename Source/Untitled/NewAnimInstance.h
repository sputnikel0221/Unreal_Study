// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Untitled.h"
#include "Animation/AnimInstance.h"
#include "NewAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLED_API UNewAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	


public:
	// 사용자정의 생성자 추가
	UNewAnimInstance();
	//  NativeUpdateAnimation(float DeltaSeconds) 선언
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsInAir;
};
