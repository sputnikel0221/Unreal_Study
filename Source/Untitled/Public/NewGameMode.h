// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "Untitled.h" //Engine과 사용자LOG 사용하기 위함
#include "GameFramework/GameModeBase.h"
#include "NewGameMode.generated.h"

UCLASS()
class UNTITLED_API ANewGameMode : public AGameModeBase
{
	GENERATED_BODY()
	ANewGameMode();

public: 
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void PostInitializeComponents() override;
	void AddScore(class ANewPlayerController* Player);

private:
	UPROPERTY()
		class ANewGameStateBase* NewGameState;

public:
	int32 GetScore() const;

private:
	UPROPERTY()
		int32 ScoreToClear;
};
