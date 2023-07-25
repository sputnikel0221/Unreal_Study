// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Untitled.h"
#include "Animation/AnimInstance.h"
#include "NewAnimInstance.generated.h"

//��������Ʈ ����
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
	// ��������� ������ �߰�
	UNewAnimInstance();
	//  NativeUpdateAnimation(float DeltaSeconds) ����
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

	//��������Ʈ ����
	FNextAttackCheckDelegate NextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;

	//��������
public:
	void SetDeadAnim() { IsDead = true; }
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;
	
	
};
