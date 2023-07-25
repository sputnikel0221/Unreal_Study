// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "Untitled.h"
#include "GameFramework/PlayerController.h"
#include "NewPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UNTITLED_API ANewPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;

protected:
	virtual void BeginPlay() override; // 없던 BeginPlay추가

public:
	ANewPlayerController();
	class UNewHUDWidget* GetHUDWidget() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UNewHUDWidget> HUDWidgetClass;

private:
	UPROPERTY()
		class UNewHUDWidget* HUDWidget;

public:
	void NPCKill(class ANewCharacter* KilledNPC) const;
private:
	UPROPERTY()
		class ANewPlayerState* NewPlayerState;

public:
	void AddGameScore() const;


protected:
	virtual void SetupInputComponent() override;
private:
	void OnGamePause();


public:
	void ChangeInputMode(bool bGameMode = true);
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UNewGamePlayWidget> MenuWidgetClass;
private:
	UPROPERTY()
		class UNewGamePlayWidget* MenuWidget;

	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;

public:
	void ShowResultUI();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UNewResultWidget> ResultWidgetClass;
private:
	UPROPERTY()
		class UNewResultWidget* ResultWidget;
};
