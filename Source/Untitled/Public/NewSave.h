// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Untitled.h"
#include "GameFramework/SaveGame.h"
#include "NewSave.generated.h"


UCLASS()
class UNTITLED_API UNewSave : public USaveGame
{
	GENERATED_BODY()
	
public:
	UNewSave();

	UPROPERTY()
		int32 Level;
	UPROPERTY()
		int32 Exp;
	UPROPERTY()
		FString PlayerName;
	UPROPERTY()
		int32 HighScore;
	UPROPERTY()
		int32 GameScore;	

	UPROPERTY()
		int32 CharacterIndex;
};
