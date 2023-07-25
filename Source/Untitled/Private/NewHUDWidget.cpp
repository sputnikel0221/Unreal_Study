// Fill out your copyright notice in the Description page of Project Settings.


#include "NewHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h" //둘을 같이 쓸 생각으로 Components.h로 묶으면 코드는 오류
#include "NewCharacterStatComponent.h"
#include "NewPlayerState.h"

// 1 - 매개변수로 받은 CharacterStat의 OnHPChanged 델리게이트에 바인딩
void UNewHUDWidget::BindCharacterStat(UNewCharacterStatComponent* CharacterStat)
{
	ABCHECK(nullptr != CharacterStat);
	CurrentCharacterStat = CharacterStat;
	CharacterStat->OnHPChanged.AddUObject(this, &UNewHUDWidget::UpdateCharacterStat);
}

// 1 - 매개변수로 받은 PlayerState의 OnPlayerStateChanged 델리게이트에 바인딩
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

// 2 - OnHPChanged에 바인딩된 함수
void UNewHUDWidget::UpdateCharacterStat()
{
	ABCHECK(CurrentCharacterStat.IsValid());

	HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());	
}

// 2 - OnPlayerStateChanged에 바인딩된 함수
void UNewHUDWidget::UpdatePlayerStat()
{
	ABCHECK(CurrentPlayerState.IsValid());

	ExpBar->SetPercent(CurrentPlayerState->GetExpRatio()); // Exp
	PlayerName->SetText(FText::FromString(CurrentPlayerState->GetPlayerName()));
	PlayerLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetCharacterLevel())));
	CurrentScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameScore())));
	HighScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameHighScore())));
}
