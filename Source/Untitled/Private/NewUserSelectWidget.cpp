// Fill out your copyright notice in the Description page of Project Settings.


#include "NewUserSelectWidget.h"
#include "NewUntitledSub.h"
#include "NewGameInstance.h"
#include "EngineUtils.h"
#include "Animation/SkeletalMeshActor.h"

// ������ ��� �߰�
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "NewSave.h"
#include "NewPlayerState.h"

void UNewUserSelectWidget::NextCharacter(bool bForward)
{
	bForward ? CurrentIndex++ : CurrentIndex--;

	// �� ó������ -1�̸� �� ���������� / �� ���������� +1�̸� �� ó������
	if (CurrentIndex == -1) CurrentIndex = MaxIndex - 1;
	if (CurrentIndex == MaxIndex) CurrentIndex = 0;

	// GetDefault
	auto CharacterSetting = GetDefault<UNewUntitledSub>();

	// Set CurrentIndex Mesh
	auto AssetRef = CharacterSetting->CharacterAssets[CurrentIndex];

	// Get GameInstance
	auto NewGameInstance = GetWorld()->GetGameInstance<UNewGameInstance>();
	ABCHECK(nullptr != NewGameInstance);
	ABCHECK(TargetComponent.IsValid());

	// GameInstance�� StreamableManager�� ���� �񵿱� ���·ε�
	USkeletalMesh* Asset = NewGameInstance->StreamableManager.LoadSynchronous<USkeletalMesh>(AssetRef);

	// ����
	if (nullptr != Asset)
	{
		TargetComponent->SetSkeletalMesh(Asset);
	}
}

void UNewUserSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentIndex = 0;

	auto CharacterSetting = GetDefault<UNewUntitledSub>();
	MaxIndex = CharacterSetting->CharacterAssets.Num();

	for (TActorIterator<ASkeletalMeshActor> IT(GetWorld()); IT; ++IT)
	{
		TargetComponent = IT->GetSkeletalMeshComponent();
		break;
	}

	// Binding (Widget & Var) by Widget Name
	PrevButton = Cast<UButton>(GetWidgetFromName(TEXT("btnPrev")));
	ABCHECK(nullptr != PrevButton);

	NextButton = Cast<UButton>(GetWidgetFromName(TEXT("btnNext")));
	ABCHECK(nullptr != NextButton);

	TextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("edtPlayerName")));
	ABCHECK(nullptr != TextBox);

	ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("btnConfirm")));
	ABCHECK(nullptr != ConfirmButton);

	// Binding Delegate 
	PrevButton->OnClicked.AddDynamic(this, &UNewUserSelectWidget::OnPrevClicked);
	NextButton->OnClicked.AddDynamic(this, &UNewUserSelectWidget::OnNextClicked);
	ConfirmButton->OnClicked.AddDynamic(this, &UNewUserSelectWidget::OnConfirmClicked);
}

void UNewUserSelectWidget::OnPrevClicked()
{
	NextCharacter(false);
}

void UNewUserSelectWidget::OnNextClicked()
{
	NextCharacter(true);
}

void UNewUserSelectWidget::OnConfirmClicked()
{
	// CharacterName�� �������� ������ ���� X
	FString CharacterName = TextBox->GetText().ToString();
	if (CharacterName.Len() <= 0 || CharacterName.Len() > 10) return;

	// PlayerData ����
	UNewSave* NewPlayerData = NewObject<UNewSave>();
	NewPlayerData->PlayerName = CharacterName;
	NewPlayerData->Level = 1;
	NewPlayerData->Exp = 0;
	NewPlayerData->GameScore = 0;
	NewPlayerData->HighScore = 0;
	NewPlayerData->CharacterIndex = CurrentIndex;

	// PlayerData ���� �� ���ӽ���
	auto NewPlayerState = GetDefault<ANewPlayerState>();
	if (UGameplayStatics::SaveGameToSlot(NewPlayerData, NewPlayerState->SaveSlotName, 0))
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("GamePlay"));
	}
	else
	{
		YU_LOG_FORMAT(Error, TEXT("Save Error !!"));
	}
}
