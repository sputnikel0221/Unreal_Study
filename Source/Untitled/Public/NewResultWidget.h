// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Untitled.h"
#include "NewGamePlayWidget.h"
#include "NewResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLED_API UNewResultWidget : public UNewGamePlayWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;


public:
	void BindGameState(class ANewGameStateBase* GameState);
private:
	TWeakObjectPtr<class ANewGameStateBase> CurrentGameState;

};
