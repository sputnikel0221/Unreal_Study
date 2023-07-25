// Fill out your copyright notice in the Description page of Project Settings.


#include "NewGameMode.h"
#include "NewPawn.h" //pawn�� �߰������ �� / ������ Character�� �߰��ؼ� �ʿ����.
#include "NewCharacter.h"
#include "NewPlayerController.h" //Controller�߰�
#include "NewPlayerState.h" //PlayerState�߰�
#include "NewGameStateBase.h" //GameStateBase�߰�


ANewGameMode::ANewGameMode()
{
	// �⺻ Pawn Ŭ������ PlayerController Ŭ������ �����Ѵ�.
	DefaultPawnClass = ANewCharacter::StaticClass(); 
	PlayerControllerClass = ANewPlayerController::StaticClass();
	PlayerStateClass = ANewPlayerState::StaticClass();
	GameStateClass = ANewGameStateBase::StaticClass();

	ScoreToClear = 100;
}

void ANewGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto NewPlayerState = Cast<ANewPlayerState>(NewPlayer->PlayerState);
	ABCHECK(nullptr != NewPlayerState);
	NewPlayerState->InitPlayerData();
}

void ANewGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	NewGameState = Cast<ANewGameStateBase>(GameState);
}

void ANewGameMode::AddScore(ANewPlayerController* Player)
{
	for (FConstPlayerControllerIterator IT = GetWorld()->GetPlayerControllerIterator(); IT; IT++)
	{
		const auto NewPlayerController = Cast<ANewPlayerController>(IT->Get());
		if ((nullptr != NewPlayerController) && (Player == NewPlayerController))
		{
			NewPlayerController->AddGameScore();
			break;
		}
	}
	NewGameState->AddGameScore();

	
	if (GetScore() >= ScoreToClear)
	{
		NewGameState->SetGameCleared();

		// World �� ��� Pawn�� ��Ȱ��ȭ
		for (FConstPawnIterator IT = GetWorld()->GetPawnIterator(); IT; ++IT)
		{
			(*IT)->TurnOff();
		}


		// World �� ��� PlayerController���� ResultUI�� ���
		for (FConstPlayerControllerIterator IT = GetWorld()->GetPlayerControllerIterator(); IT; ++IT)
		{
			const auto NewPlayerController = Cast<ANewPlayerController>(IT->Get());
			if (nullptr != NewPlayerController)
			{
				NewPlayerController->ShowResultUI();
			}
		}
	}
}

int32 ANewGameMode::GetScore() const
{
	return NewGameState->GetTotalGameScore();
}
