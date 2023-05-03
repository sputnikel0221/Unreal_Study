// Fill out your copyright notice in the Description page of Project Settings.


#include "NewPlayerController.h"

void ANewPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	YU_LOG_FORMAT(Warning, TEXT("PLayer Controller"));
}

void ANewPlayerController::OnPossess(APawn* aPawn)
{
	YU_LOG_FORMAT(Warning, TEXT("PLayer Controller Possess"));
	Super::OnPossess(aPawn);
}
