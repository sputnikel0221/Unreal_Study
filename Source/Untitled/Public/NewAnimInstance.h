// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Untitled.h"
#include "Animation/AnimInstance.h"
#include "NewAnimInstance.generated.h"

//델리게이트 생성
DECLARE_MULTICAST_DELEGATE(FNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

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

	void PlayAttackMontage();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsInAir;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* MyAttackMontage;

private:
	UFUNCTION()
		void AnimNotify_AttackHitCheck();
	UFUNCTION()
		void AnimNotify_NextAttackCheck();

	FName GetAttackMontageSectionName(int32 Section);

public:
	void JumpToAttackMontageSection(int32 NewSection);

	//델리게이트 연결
	FNextAttackCheckDelegate NextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;

	//죽음관련
public:
	void SetDeadAnim() { IsDead = true; }
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;
	
	
};
