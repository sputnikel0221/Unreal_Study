// Fill out your copyright notice in the Description page of Project Settings.


#include "NewAnimInstance.h"

UNewAnimInstance::UNewAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Script/Engine.AnimMontage'/Game/Book/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		MyAttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UNewAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (!IsValid(Pawn)) return;

	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();

		// 점프 코드
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UNewAnimInstance::PlayAttackMontage()
{
		Montage_Play(MyAttackMontage, 1.0f); //이걸 해야만 몽타주가 실행됨
}

// == Combo Attack ==
FName UNewAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attack%d"),Section));
}

void UNewAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), 
		MyAttackMontage);
}

void UNewAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UNewAnimInstance::AnimNotify_NextAttackCheck()
{
	NextAttackCheck.Broadcast();
}



