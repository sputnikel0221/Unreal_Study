// Fill out your copyright notice in the Description page of Project Settings.


#include "NewResultWidget.h"
#include "Components/TextBlock.h"
#include "NewGameStateBase.h"

void UNewResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ABCHECK(CurrentGameState.IsValid()); //�߰�

	auto Result = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtResult")));
	ABCHECK(nullptr != Result);

	auto TotalScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTotalScore")));
	ABCHECK(nullptr != TotalScore);

	// ���� �� �ο�
	Result->SetText(FText(FText::FromString(CurrentGameState->IsGameCleared()  ?
		TEXT("Mission Complete!!") : TEXT("Mission Failed"))));
	TotalScore->SetText(FText(FText::FromString(FString::FromInt(CurrentGameState->GetTotalGameScore()))));
}

void UNewResultWidget::BindGameState(ANewGameStateBase* GameState)
{
	ABCHECK(nullptr != GameState);
	CurrentGameState = GameState;
}
