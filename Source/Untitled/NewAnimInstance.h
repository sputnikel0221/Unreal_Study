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
	// ��������� ������ �߰�
	UNewAnimInstance();
	//  NativeUpdateAnimation(float DeltaSeconds) ����
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsInAir;
};
