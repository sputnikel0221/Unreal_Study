// Fill out your copyright notice in the Description page of Project Settings.


#include "NewGamePlayWidget.h"
#include "Components/Button.h"
#include "NewPlayerController.h"

void UNewGamePlayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (nullptr != ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UNewGamePlayWidget::OnResumeClicked);
	}

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	ABCHECK(nullptr != ReturnToTitleButton);

	ReturnToTitleButton->OnClicked.AddDynamic(this, &UNewGamePlayWidget::OnReturnToTitleClicked);

	RetryGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnRetryGame")));
	if (nullptr != RetryGameButton)
	{
		RetryGameButton->OnClicked.AddDynamic(this, &UNewGamePlayWidget::OnRetryGameClicked);
	}

}

void UNewGamePlayWidget::OnResumeClicked()
{
	auto NewPlayerController = Cast<ANewPlayerController>(GetOwningPlayer()); // ���� UI�� ���� �ִ� PlayerController
	ABCHECK(nullptr != NewPlayerController);
	
	RemoveFromParent(); // ����Ʈ�� ����� ������ ����	

	NewPlayerController->ChangeInputMode(true);
	NewPlayerController->SetPause(false);
}

void UNewGamePlayWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}

void UNewGamePlayWidget::OnRetryGameClicked()
{
	auto NewPlayerController = Cast<ANewPlayerController>(GetOwningPlayer());
	ABCHECK(nullptr != NewPlayerController);

	NewPlayerController->RestartLevel();
}
