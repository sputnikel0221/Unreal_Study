// Fill out your copyright notice in the Description page of Project Settings.


#include "NewHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h" //���� ���� �� �������� Components.h�� ������ �ڵ�� ����
#include "NewCharacterStatComponent.h"
#include "NewPlayerState.h"

// 1 - �Ű������� ���� CharacterStat�� OnHPChanged ��������Ʈ�� ���ε�
void UNewHUDWidget::BindCharacterStat(UNewCharacterStatComponent* CharacterStat)
{
	ABCHECK(nullptr != CharacterStat);
	CurrentCharacterStat = CharacterStat;
	CharacterStat->OnHPChanged.AddUObject(this, &UNewHUDWidget::UpdateCharacterStat);
}

// 1 - �Ű������� ���� PlayerState�� OnPlayerStateChanged ��������Ʈ�� ���ε�
void UNewHUDWidget::BindPlayerState(ANewPlayerState* PlayerState)
{
	ABCHECK(nullptr != PlayerState);
	CurrentPlayerState = PlayerState;
	PlayerState->OnPlayerStateChanged.AddUObject(this, &UNewHUDWidget::UpdatePlayerStat);
}

void UNewHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));
	ABCHECK(nullptr != HPBar);
	
	ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbExp")));
	ABCHECK(nullptr != ExpBar);

	PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerName")));
	ABCHECK(nullptr != PlayerName);
	
	PlayerLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtLevel")));
	ABCHECK(nullptr != PlayerLevel);

	CurrentScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtCurrentScore")));
	ABCHECK(nullptr != CurrentScore);

	HighScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtHighScore")));
	ABCHECK(nullptr != HighScore);
}

// 2 - OnHPChanged�� ���ε��� �Լ�
void UNewHUDWidget::UpdateCharacterStat()
{
	ABCHECK(CurrentCharacterStat.IsValid());

	HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());	
}

// 2 - OnPlayerStateChanged�� ���ε��� �Լ�
void UNewHUDWidget::UpdatePlayerStat()
{
	ABCHECK(CurrentPlayerState.IsValid());

	ExpBar->SetPercent(CurrentPlayerState->GetExpRatio()); // Exp
	PlayerName->SetText(FText::FromString(CurrentPlayerState->GetPlayerName()));
	PlayerLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetCharacterLevel())));
	CurrentScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameScore())));
	HighScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameHighScore())));
}
