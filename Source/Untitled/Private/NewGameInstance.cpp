// Fill out your copyright notice in the Description page of Project Settings.


#include "NewGameInstance.h"

UNewGameInstance::UNewGameInstance()
{

	FString CharacterDataPath = TEXT("/Script/Engine.DataTable'/Game/Book/GameData/NewCharacterData.NewCharacterData'");

	static ConstructorHelpers::FObjectFinder<UDataTable>DT_NewCharacter(*CharacterDataPath);
	ABCHECK(DT_NewCharacter.Succeeded());
	NewCharacterTable = DT_NewCharacter.Object;
	ABCHECK(NewCharacterTable->GetRowMap().Num() > 0);

	YU_LOG_FORMAT(Warning, TEXT("UNewGameInstanceConstructor-End"));
}

void UNewGameInstance::Init()
{
	Super::Init();
	
}

FNewCharacterData* UNewGameInstance::GetNewCharacterData(int32 Level)
{
	return NewCharacterTable->FindRow<FNewCharacterData>(*FString::FromInt(Level), TEXT(""));
}