// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NewUntitledSub.generated.h"

UCLASS(config = UntitledSub) // 1. �ش� �ڵ�� config������ DefaultUntitledSub.ini������ ���� Ŭ�������� ����� �� �ְ� �ȴ�.
class UNTITLEDSUB_API UNewUntitledSub : public UObject
{
	GENERATED_BODY()
	
public:
	UNewUntitledSub();

	UPROPERTY(config) // 2. ini������ ����Ѵ�.
		TArray<FSoftObjectPath> CharacterAssets; //���������� ini������ ���� ����
};
