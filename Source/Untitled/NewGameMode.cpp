// Fill out your copyright notice in the Description page of Project Settings.


#include "NewGameMode.h"
#include "NewPawn.h" //pawn을 추가해줘야 함 / 이제는 Character를 추가해서 필요없다.
#include "NewCharacter.h"
#include "NewPlayerController.h" //Controller추가


ANewGameMode::ANewGameMode()
{
	DefaultPawnClass = ANewCharacter::StaticClass(); // 해당부분을 바꾼다.
	PlayerControllerClass = ANewPlayerController::StaticClass();
}

void ANewGameMode::PostLogin(APlayerController* NewPlayer)
{
	YU_LOG_FORMAT(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	YU_LOG_FORMAT(Warning, TEXT("PostLogin End"));
}
