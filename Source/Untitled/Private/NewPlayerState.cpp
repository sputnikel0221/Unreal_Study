// Fill out your copyright notice in the Description page of Project Settings.


#include "NewPlayerState.h"
#include "NewGameInstance.h"
#include "NewSave.h"

//��� �̰� ��� �Ǵ� ���� SaveSlotName�� �ʿ���
ANewPlayerState::ANewPlayerState()
{
	GameScore = 0;
	CharacterLevel = 5;
	Exp = 0;
	GameHighScore = 0;
	SaveSlotName = TEXT("Player1");
	CharacterIndex = 0;
}

int32 ANewPlayerState::GetGameScore() const
{

	return GameScore;
}

int32 ANewPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

void ANewPlayerState::InitPlayerData()
{
	auto NewSave = Cast<UNewSave>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (nullptr == NewSave)
	{
		NewSave = GetMutableDefault<UNewSave>();
	}

	SetPlayerName(NewSave->PlayerName);
	SetCharacterLevel(5);
	GameScore = NewSave->GameScore;
	GameHighScore = NewSave->HighScore;
	Exp = 440; //NewSave->Exp;
	CharacterIndex = NewSave->CharacterIndex;

	SavePlayerData();
}

float ANewPlayerState::GetExpRatio() const
{
	if (CurrentStatData->NextExp <= KINDA_SMALL_NUMBER) 
		return 0.0f;

	float Result = (float)Exp / (float)CurrentStatData->NextExp;
	YU_LOG_FORMAT(Warning, TEXT("Ratio: %f, Current: %d, Next: %d"), Result, Exp, CurrentStatData->NextExp);

	return Result;
}

bool ANewPlayerState::AddExp(int32 IncomeExp)
{
	if (CurrentStatData->NextExp == -1) 
		return false;
	
	bool DidLevelUp = false;
	Exp = Exp + IncomeExp;
	if (Exp >= CurrentStatData->NextExp)
	{
		Exp -= CurrentStatData->NextExp; 
		SetCharacterLevel(CharacterLevel + 1);
		DidLevelUp = true;
	}

	OnPlayerStateChanged.Broadcast(); //UI �̸�,����,���� �����ϴ� �Լ� ȣ��
	SavePlayerData();
	return DidLevelUp;
}

void ANewPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	auto NewGameInstance = Cast<UNewGameInstance>(GetGameInstance());
	ABCHECK(nullptr != NewGameInstance);

	CurrentStatData = NewGameInstance->GetNewCharacterData(NewCharacterLevel);
	ABCHECK(nullptr != CurrentStatData);
	
	CharacterLevel = NewCharacterLevel;
}

void ANewPlayerState::AddGameScore()
{
	GameScore+=10;
	if (GameScore > GameHighScore)
	{
		GameHighScore = GameScore;
	}
	OnPlayerStateChanged.Broadcast();
	SavePlayerData();
}

int32 ANewPlayerState::GetGameHighScore() const
{
	return GameHighScore;
}

int32 ANewPlayerState::GetCharacterIndex() const
{
	return CharacterIndex;
}

void ANewPlayerState::SavePlayerData()
{
	UNewSave* NewPlayerData = NewObject<UNewSave>();
	NewPlayerData->PlayerName = GetPlayerName();
	NewPlayerData->Level = CharacterLevel;
	NewPlayerData->Exp = Exp;
	NewPlayerData->GameScore = GameScore; // ���� GameScore�� �����
	NewPlayerData->HighScore = GameHighScore;
	NewPlayerData->CharacterIndex = CharacterIndex;

	if (!UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
	{
		YU_LOG_FORMAT(Error, TEXT("Save Error"));
	}
}

void ANewPlayerState::DeadPlayerData()
{
	UNewSave* NewPlayerData = NewObject<UNewSave>();
	NewPlayerData->PlayerName = GetPlayerName();
	NewPlayerData->Level = 5; // ������ ó�� ������ �ٲ�
	NewPlayerData->Exp = 0; // ������ 0���� �ٲ�
	NewPlayerData->GameScore = 0; // ������ 0���� �ٲ�, �������� 0���� �ٲ��� ����
	NewPlayerData->HighScore = GameHighScore;

	if (!UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
	{
		YU_LOG_FORMAT(Error, TEXT("DEAD : Save Error"));
	}
}