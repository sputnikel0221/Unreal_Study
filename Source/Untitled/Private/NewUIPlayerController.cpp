// Fill out your copyright notice in the Description page of Project Settings.


#include "NewUIPlayerController.h"
#include "Blueprint/UserWidget.h" // Ãß°¡ 

void ANewUIPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ABCHECK(nullptr != UIWidgetClass);

	UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
	ABCHECK(nullptr != UIWidgetInstance);

	UIWidgetInstance->AddToViewport();

	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
	SetInputMode(Mode);
	bShowMouseCursor = true;
}
