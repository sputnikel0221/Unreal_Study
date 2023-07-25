// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Untitled.h"
#include "Engine/DataTable.h" //FTableRowBase
#include "Engine/GameInstance.h" 
#include "Engine/StreamableManager.h"
#include "NewGameInstance.generated.h"


USTRUCT(BlueprintType)
struct FNewCharacterData : public FTableRowBase
{
	GENERATED_BODY();

public:
	FNewCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float MaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		float Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 DropExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		int32 NextExp;
};

UCLASS()
class UNTITLED_API UNewGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:
	UNewGameInstance();

	virtual void Init() override;
	FNewCharacterData* GetNewCharacterData(int32 Level);

private:
	UPROPERTY()
		class UDataTable* NewCharacterTable; //테이블데이터 관리

public:
	FStreamableManager StreamableManager;

};


