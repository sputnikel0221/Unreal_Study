// Fill out your copyright notice in the Description page of Project Settings.

#include "NewPlayerController.h"
#include "NewHUDWidget.h"
#include "NewPlayerState.h"
#include "NewCharacter.h"
#include "NewGamePlayWidget.h"
#include "NewResultWidget.h"
#include "NewGameStateBase.h"

void ANewPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	YU_LOG_FORMAT(Warning, TEXT("PLayer Controller"));
}

void ANewPlayerController::OnPossess(APawn* aPawn)
{
	YU_LOG_FORMAT(Warning, TEXT("PLayer Controller Possess"));
	Super::OnPossess(aPawn);
}

void ANewPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	FInputModeGameOnly NewInputMode;
	SetInputMode(NewInputMode);
	
	ChangeInputMode(true); //추가

	// 위젯 생성
	HUDWidget = CreateWidget<UNewHUDWidget>(this, HUDWidgetClass); //생성되었으므로 NativeConstructor가 실행될듯
	ABCHECK(nullptr != HUDWidget);

	HUDWidget->AddToViewport(1);  // 추가 : 왜 1을 넣었을까??
	

	// 위젯 바인딩
	NewPlayerState = Cast<ANewPlayerState>(PlayerState); 
	ABCHECK(nullptr != NewPlayerState);

	HUDWidget->BindPlayerState(NewPlayerState);
	

	NewPlayerState->OnPlayerStateChanged.Broadcast(); // 바로 델리게이트 호출


	//
	ResultWidget = CreateWidget<UNewResultWidget>(this, ResultWidgetClass);
	ABCHECK(nullptr != ResultWidget);
}

ANewPlayerController::ANewPlayerController()
{
	// 위젯 에셋 클래스 설정
	static ConstructorHelpers::FClassFinder<UNewHUDWidget> UI_HUD_C(TEXT("/Game/Book/UI/UI_HUD"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}

	// CPP를 상속받는 Blueprint를 생성하지 않으면 직접 이렇게 설정
	static ConstructorHelpers::FClassFinder<UNewGamePlayWidget> UI_MENU_C(TEXT("/Game/Book/UI/UI_Menu"));
	if (UI_MENU_C.Succeeded())
	{
		MenuWidgetClass = UI_MENU_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UNewGamePlayWidget> UI_RESULT_C(TEXT("/Game/Book/UI/UI_Result"));
	if (UI_RESULT_C.Succeeded())
	{
		ResultWidgetClass = UI_RESULT_C.Class;
	}
}

UNewHUDWidget* ANewPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

void ANewPlayerController::NPCKill(ANewCharacter* KilledNPC) const
{
	NewPlayerState->AddExp(KilledNPC->GetExp());
}

void ANewPlayerController::AddGameScore() const
{
	NewPlayerState->AddGameScore();
}

void ANewPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &ANewPlayerController::OnGamePause);
}

void ANewPlayerController::OnGamePause()
{
	MenuWidget = CreateWidget<UNewGamePlayWidget>(this, MenuWidgetClass);
	ABCHECK(nullptr != MenuWidget);
	MenuWidget->AddToViewport(3);

	SetPause(true); // 게임 멈춤
	ChangeInputMode(false);
}

void ANewPlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}

void ANewPlayerController::ShowResultUI()
{
	//이렇게 GameState를 불러온다.
	auto NewGameStateBase = Cast<ANewGameStateBase>(UGameplayStatics::GetGameState(this));
	ABCHECK(nullptr != NewGameStateBase);
	ResultWidget->BindGameState(NewGameStateBase); //gamestate를 연결한 위젯

	ResultWidget->AddToViewport();
	ChangeInputMode(false);
}
