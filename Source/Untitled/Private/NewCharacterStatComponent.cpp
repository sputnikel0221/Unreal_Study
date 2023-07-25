// Fill out your copyright notice in the Description page of Project Settings.


#include "NewCharacterStatComponent.h"
#include "NewGameInstance.h"

// Sets default values for this component's properties
// 객체를 생성하면 Level은 1로 초기화
UNewCharacterStatComponent::UNewCharacterStatComponent()
{

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	Level = 1;
}


// Called when the game starts
void UNewCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

// 생성자에서 설정한 1레벨을 SetNewLevel에 넣어 초기화
// NewCharacterStat의 멤버변수가 이제서야 모두 초기화
void UNewCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	YU_LOG_FORMAT(Warning, TEXT("%d"), Level);
	SetNewLevel(Level); 
}


void UNewCharacterStatComponent::SetNewLevel(int32 NewLevel)
{

	auto NewGameInstance = Cast<UNewGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	ABCHECK(nullptr != NewGameInstance);
	CurrentStatData = NewGameInstance->GetNewCharacterData(NewLevel);  //해당 레벨의 데이터
	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;

		//CurrentHP = CurrentStatData->MaxHP;
		SetHP(CurrentStatData->MaxHP);

		YU_LOG_FORMAT(Error, TEXT("GameInstance Is Successfully Work !!"));
		YU_LOG_FORMAT(Error, TEXT("Lv %d. MaxHP : %f, DropEXP : %d"), 
			NewGameInstance->GetNewCharacterData(1)->Level, 
			NewGameInstance->GetNewCharacterData(1)->MaxHP,
			NewGameInstance->GetNewCharacterData(1)->DropExp);
	}
	else
	{
		YU_LOG_FORMAT(Warning, TEXT("Level Not Exist"));
	}
}

void UNewCharacterStatComponent::SetDamage(float NewDamage)
{
	ABCHECK(nullptr != CurrentStatData);
	//CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP);
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
}

float UNewCharacterStatComponent::GetAttack()
{
	ABCHECK(nullptr != CurrentStatData, 0.0f);
	return CurrentStatData->Attack;
}

void UNewCharacterStatComponent::SetHP(float NewHP)
{

	CurrentHP = NewHP;
	OnHPChanged.Broadcast();

	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

float UNewCharacterStatComponent::GetHPRatio()
{
	ABCHECK(nullptr != CurrentStatData, 0.0f);

	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}

int32 UNewCharacterStatComponent::GetDropExp() const
{
	return CurrentStatData->DropExp;
}

