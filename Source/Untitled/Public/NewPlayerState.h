// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Untitled.h"
#include "GameFramework/PlayerState.h"
#include "NewPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);

UCLASS()
class UNTITLED_API ANewPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ANewPlayerState();

	int32 GetGameScore() const;
	int32 GetCharacterLevel() const;

	void InitPlayerData();

protected:
	UPROPERTY(Transient)
		int32 GameScore;
	UPROPERTY(Transient)
		int32 CharacterLevel;
public:
	FOnPlayerStateChangedDelegate OnPlayerStateChanged;

public:
	float GetExpRatio() const;
	bool AddExp(int32 IncomeExp);

protected:
	UPROPERTY(Transient)
		int32 Exp;
private:
	void SetCharacterLevel(int32 NewCharacterLevel);
	struct FNewCharacterData* CurrentStatData;

public:
	void AddGameScore();

public:
	int32 GetGameHighScore() const;
	FString SaveSlotName;
	int32 GetCharacterIndex() const;

protected:
	UPROPERTY(Transient)
		int32 GameHighScore;

	UPROPERTY(Transient)
		int32 CharacterIndex;

public:
	void SavePlayerData();
	void DeadPlayerData();
};
