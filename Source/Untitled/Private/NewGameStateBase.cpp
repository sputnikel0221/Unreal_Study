// Fill out your copyright notice in the Description page of Project Settings.


#include "NewGameStateBase.h"
#include "NewPlayerController.h"
#include "NewPlayerState.h"

#include "Kismet/GameplayStatics.h"

ANewGameStateBase::ANewGameStateBase()
{
	TotalGameScore = 0;
	bGameCleared = false;
}

int32 ANewGameStateBase::GetTotalGameScore() const
{
	return TotalGameScore;
}

void ANewGameStateBase::AddGameScore()
{
	TotalGameScore+=10;
}

void ANewGameStateBase::SetTotalScore(int32 Score)
{
	TotalGameScore = Score;
}

void ANewGameStateBase::SetGameCleared()
{
	bGameCleared = true;
}

bool ANewGameStateBase::IsGameCleared() const
{
	return bGameCleared;
}
