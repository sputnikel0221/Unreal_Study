// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Untitled.h"
#include "Blueprint/UserWidget.h"
#include "NewHUDWidget.generated.h"


UCLASS()
class UNTITLED_API UNewHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class UNewCharacterStatComponent* CharacterStat);
	void BindPlayerState(class ANewPlayerState* PlayerState);

protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
	void UpdatePlayerStat();

private:
	TWeakObjectPtr<class UNewCharacterStatComponent> CurrentCharacterStat;
	TWeakObjectPtr<class ANewPlayerState> CurrentPlayerState;

	UPROPERTY()
		class UProgressBar* HPBar;
	UPROPERTY()
		class UProgressBar* ExpBar;
	UPROPERTY()
		class UTextBlock* PlayerName;
	UPROPERTY()
		class UTextBlock* PlayerLevel;
	UPROPERTY()
		class UTextBlock* CurrentScore;
	UPROPERTY()
		class UTextBlock* HighScore;
};
