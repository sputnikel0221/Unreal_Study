// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Untitled.h"
#include "GameFramework/GameStateBase.h"
#include "NewGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLED_API ANewGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	

public:
	ANewGameStateBase();
	
public:
	int32 GetTotalGameScore() const;
	void AddGameScore();

private:
	UPROPERTY(Transient)
		int32 TotalGameScore;

public:
	void SetTotalScore(int32 Score);

public:
	void SetGameCleared();
	bool IsGameCleared() const;

private:
	UPROPERTY(Transient)
		bool bGameCleared;
};
