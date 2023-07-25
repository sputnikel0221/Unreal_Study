// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Untitled.h"
#include "Blueprint/UserWidget.h"
#include "NewUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLED_API UNewUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindCharacterStat(class UNewCharacterStatComponent* NewCharacterStat);
private:
	TWeakObjectPtr<class UNewCharacterStatComponent> CurrentCharacterStat;
	
protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();
private:
	UPROPERTY()
		class UProgressBar* HPProgressBar;
};
