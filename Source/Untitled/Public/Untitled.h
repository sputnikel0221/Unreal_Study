// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

//#include "CoreMinimal.h"

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(Untitled, Log, All);

#define CODE_LINE (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define YU_LOG(Var) UE_LOG(Untitled, Var, TEXT("%s"), *CODE_LINE)
#define YU_LOG_FORMAT(Var, Format, ...) UE_LOG(Untitled, Var, TEXT("%s %s"), *CODE_LINE, *FString::Printf(Format, ##__VA_ARGS__)) //가변인자매크로

#define ABCHECK(Expr, ...) {if(!(Expr)) {YU_LOG_FORMAT(Error, TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	PREINIT,
	LOADING,
	READY,
	DEAD
};


