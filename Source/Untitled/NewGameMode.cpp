// Fill out your copyright notice in the Description page of Project Settings.


#include "NewGameMode.h"
#include "NewPawn.h" //pawn�� �߰������ �� / ������ Character�� �߰��ؼ� �ʿ����.
#include "NewCharacter.h"
#include "NewPlayerController.h" //Controller�߰�


ANewGameMode::ANewGameMode()
{
	DefaultPawnClass = ANewCharacter::StaticClass(); // �ش�κ��� �ٲ۴�.
	PlayerControllerClass = ANewPlayerController::StaticClass();
}

void ANewGameMode::PostLogin(APlayerController* NewPlayer)
{
	YU_LOG_FORMAT(Warning, TEXT("PostLogin Begin"));
	Super::PostLogin(NewPlayer);
	YU_LOG_FORMAT(Warning, TEXT("PostLogin End"));
}
