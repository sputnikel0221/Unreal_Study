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
};
