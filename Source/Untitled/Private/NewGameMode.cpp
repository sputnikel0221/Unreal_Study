// Fill out your copyright notice in the Description page of Project Settings.


#include "NewGameMode.h"
#include "NewPawn.h" //pawn을 추가해줘야 함 / 이제는 Character를 추가해서 필요없다.
#include "NewCharacter.h"
#include "NewPlayerController.h" //Controller추가
#include "NewPlayerState.h" //PlayerState추가
#include "NewGameStateBase.h" //GameStateBase추가


ANewGameMode::ANewGameMode()
{
	// 기본 Pawn 클래스와 PlayerController 클래스를 설정한다.
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

		// World 내 모든 Pawn을 비활성화
		for (FConstPawnIterator IT = GetWorld()->GetPawnIterator(); IT; ++IT)
		{
			(*IT)->TurnOff();
		}


		// World 내 모든 PlayerController에게 ResultUI를 띄움
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
