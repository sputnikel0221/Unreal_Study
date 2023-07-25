// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NewUntitledSub.generated.h"

UCLASS(config = UntitledSub) // 1. 해당 코드로 config폴더의 DefaultUntitledSub.ini파일을 현재 클래스에서 사용할 수 있게 된다.
class UNTITLEDSUB_API UNewUntitledSub : public UObject
{
	GENERATED_BODY()
	
public:
	UNewUntitledSub();

	UPROPERTY(config) // 2. ini파일을 사용한다.
		TArray<FSoftObjectPath> CharacterAssets; //최종적으로 ini파일이 들어가는 변수
};
