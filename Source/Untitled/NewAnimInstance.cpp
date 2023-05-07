// Fill out your copyright notice in the Description page of Project Settings.


#include "NewAnimInstance.h"

UNewAnimInstance::UNewAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
}

void UNewAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
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
