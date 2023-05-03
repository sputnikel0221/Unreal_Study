// Fill out your copyright notice in the Description page of Project Settings.


#include "NewGameMode.h"
#include "NewPawn.h" //pawn을 추가해줘야 함
#include "NewPlayerController.h" //Controller추가


ANewGameMode::ANewGameMode()
{
	DefaultPawnClass = ANewPawn::StaticClass();
	PlayerControllerClass = ANewPlayerController::StaticClass();
}

void ANewGameMode::PostLogin(APlayerController* NewPlayer)
{
	YU_LOG_FORMAT(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	YU_LOG_FORMAT(Warning, TEXT("PostLogin End"));
}
