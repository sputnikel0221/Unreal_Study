// Fill out your copyright notice in the Description page of Project Settings.


#include "NewGameMode.h"
#include "NewPawn.h" //pawn�� �߰������ ��
#include "NewPlayerController.h" //Controller�߰�


ANewGameMode::ANewGameMode()
{
	DefaultPawnClass = ANewPawn::StaticClass();
	PlayerControllerClass = ANewPlayerController::StaticClass();
}
