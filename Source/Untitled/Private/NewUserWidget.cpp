// Fill out your copyright notice in the Description page of Project Settings.


#include "NewUserWidget.h"
#include "NewCharacterStatComponent.h"
#include "Components/ProgressBar.h"

void UNewUserWidget::BindCharacterStat(UNewCharacterStatComponent* NewCharacterStat)
{
	ABCHECK(nullptr != NewCharacterStat);

	CurrentCharacterStat = NewCharacterStat;

	//Delegate ¿¬°á
	NewCharacterStat->OnHPChanged.AddUObject(this, &UNewUserWidget::UpdateHPWidget); 
}

void UNewUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	UpdateHPWidget();
}

void UNewUserWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}
