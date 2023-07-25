// Fill out your copyright notice in the Description page of Project Settings.


#include "NewCharacter.h"
#include "NewCharacterStatComponent.h"
#include "NewAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "NewWeapon.h"
#include "Engine/DamageEvents.h" //UE5에선 추가
#include "Components/WidgetComponent.h"
#include "NewUserWidget.h"
#include "NewAIController.h"
#include "NewUntitledSub.h"
#include "NewGameInstance.h"
#include "NewPlayerController.h"
#include "NewPlayerState.h"
#include "NewHUDWidget.h"
#include "NewGameMode.h"
#include "NewGameStateBase.h"
#include "VisualLogger/VisualLogger.h"

// Sets default values
ANewCharacter::ANewCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	// Capsule이 RootComponent, Get으로 가져온다.
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	// Capsule의 세부 설정을 하지 않는다 왜지?

	//HPBarWidget은 다른 위젯과 다르게 Character의 Mesh에 부착되고, 작게 사이즈 조정되어 띄워진다.
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	HPBarWidget->SetupAttachment(GetMesh());

	// 아래의 설정들은 GetMesh() 외에 다 같다.
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	// * 없어져도 되는 부분
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_PAWNBOARD(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	if (SK_PAWNBOARD.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_PAWNBOARD.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);	// BP버전 (통상적으로 쓰는 방법)

	// BP로부터 Animation Instance 클래스를 찾아서 가져옴
	static ConstructorHelpers::FClassFinder<UAnimInstance> NewAnimInstance(TEXT("/Script/Engine.AnimBlueprint'/Game/Book/BP_WarriorAnim.BP_WarriorAnim_C'"));
	{
		GetMesh()->SetAnimInstanceClass(NewAnimInstance.Class);
	}

	SetControlMode(ControlMode::GTA);

	// InterpTo를 위한 속도값 저장
	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;

	// Character Movement의 Jump수치를 바꿈
	GetCharacterMovement()->JumpZVelocity = 800.0f;

	IsAttacking = false;

	MaxCombo = 4;
	AttackEndComboState();

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("NewCharacterChannel"));

	AttackRadius = 50.0f;
	AttackRange = 80.0f; // 200 -> 80


	CharacterStat = CreateDefaultSubobject<UNewCharacterStatComponent>(TEXT("CHARACTERSTAT"));


	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/Book/UI/UI_HPBar"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}


	AIControllerClass = ANewAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	AssetIndex = 4;

	SetActorHiddenInGame(true);
	HPBarWidget->SetHiddenInGame(true);
	SetCanBeDamaged(false);

	DeadTimer = 5.0f;
}
//여기까지 생성자

// Called when the game starts or when spawned
void ANewCharacter::BeginPlay()
{
	Super::BeginPlay();

	// PlayerController인지 확인
	bIsPlayer = IsPlayerControlled();

	// 1. bIsPlayer값에 따라 - NewPlayerController / NewAIController의 값이 부여됨
	if (bIsPlayer)
	{
		NewPlayerController = Cast<ANewPlayerController>(GetController());
		ABCHECK(nullptr != NewPlayerController);
	}
	else
	{
		NewAIController = Cast<ANewAIController>(GetController());
		ABCHECK(nullptr != NewAIController);
	}


	// 2. bIsPlayer값에 따라 - 에셋의 인덱스 지정
	// CDO를 가져옴
	auto DefaultSetting = GetDefault<UNewUntitledSub>();

	if (bIsPlayer)
	{
		auto NewPlayerState = Cast<ANewPlayerState>(NewPlayerController->PlayerState);
		ABCHECK(nullptr != NewPlayerState);
		AssetIndex = NewPlayerState->GetCharacterIndex();
	}
	else
	{
		// 랜덤 에셋경로 가져오기
		AssetIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
		YU_LOG_FORMAT(Error, TEXT("%d AIController======"), AssetIndex);
	}

	// 에셋 인덱스에 맞는 에셋의 경로를 가져옴
	CharacterAssetToLoad = DefaultSetting->CharacterAssets[AssetIndex];
	
	auto NewGameInstance = Cast<UNewGameInstance>(GetGameInstance());
	ABCHECK(nullptr != NewGameInstance);

	// 에셋 비동기로드 후 델리게이트 호출
	AssetStreamingHandle = NewGameInstance->StreamableManager.RequestAsyncLoad(
		CharacterAssetToLoad,
		FStreamableDelegate::CreateUObject(this, &ANewCharacter::OnAssetLoadCompleted)
	);

	// LOADING 상태로 전환
	SetCharacterState(ECharacterState::LOADING);

}

// Called every frame
void ANewCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//float를 위한 InterpTo 사용
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLength, DeltaTime, ArmLengthSpeed);

	switch (CurrentControlMode) //실제 이동의 담당
	{
	case ControlMode::DIABLO:// 입력으로 받은 X,Y값(Z는 위라서 제외)을 확인하고 맞게 이동
		//GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
		//AddMovementInput(DirectionToMove);

		// Rotatoion를 위한 InterpTo 사용
		FRotator tmp = FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotation, DeltaTime, ArmRotationSpeed);
		SpringArm->SetRelativeRotation(tmp);

		// 이동관련
		GetController()->SetControlRotation(DirectionToMove.ToOrientationRotator());
		AddMovementInput(DirectionToMove); //이동
		break;
	}

}

// Called to bind functionality to input
void ANewCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &ANewCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("RightLeft"), this, &ANewCharacter::RightLeft);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ANewCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ANewCharacter::Turn);

	// 액션을 Bind
	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &ANewCharacter::ViewChange);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ANewCharacter::Attack);
}

// 생성자 이후
void ANewCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	NewAnim = Cast<UNewAnimInstance>(GetMesh()->GetAnimInstance());

	NewAnim->OnMontageEnded.AddDynamic(this, &ANewCharacter::OnAttackMontageEnded);

	// NextAttackCheck델리게이트에 Lambda함수 생성과 동시에 바로연결
	NewAnim->NextAttackCheck.AddLambda([this]() -> void {
		CanNextCombo = false;
	if (IsComboInputOn)
	{
		AttackStartComboState();
		NewAnim->JumpToAttackMontageSection(CurrentCombo);
	}

		});

	NewAnim->OnAttackHitCheck.AddUObject(this, &ANewCharacter::AttackCheck); //델리게이트 바인딩

	/* 
	삭제해도 되는 부분?
	CharacterStat->OnHPIsZero.AddLambda([this]()->void {
		YU_LOG_FORMAT(Warning, TEXT("OnHPIsZero"));
	NewAnim->SetDeadAnim();
	SetActorEnableCollision(false);
		});

	HPBarWidget->InitWidget(); //오류해결
	auto CharacterWidget = Cast<UNewUserWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}
	*/

}


void ANewCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	AttackEndComboState();

	OnAttackEnd.Broadcast();
}

void ANewCharacter::UpDown(float UDValue)
{
	switch (CurrentControlMode)
	{
	case ControlMode::GTA:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), UDValue);
		break;
	case ControlMode::DIABLO:
		DirectionToMove.X = UDValue;
		break;
	}
	//AddMovementInput(GetActorForwardVector(), UDValue);

}

void ANewCharacter::RightLeft(float RLValue)
{
	switch (CurrentControlMode)
	{
	case ControlMode::GTA:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), RLValue);
		break;
	case ControlMode::DIABLO:
		DirectionToMove.Y = RLValue;
		break;
		//AddMovementInput(GetActorRightVector(), RLValue);
	}
}

void ANewCharacter::LookUp(float aValue)
{
	switch (CurrentControlMode)
	{
	case ControlMode::GTA:
		AddControllerPitchInput(aValue);
		break;
	}
}

void ANewCharacter::Turn(float aValue)
{
	switch (CurrentControlMode)
	{
	case ControlMode::GTA:
		AddControllerYawInput(aValue);
		break;
	}
}

void ANewCharacter::Attack()
{
	if (IsAttacking)
	{
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		AttackStartComboState();
		NewAnim->PlayAttackMontage();
		NewAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}

void ANewCharacter::OnAssetLoadCompleted()
{
	AssetStreamingHandle->ReleaseHandle();
	TSoftObjectPtr<USkeletalMesh>LoadedAssetPath(CharacterAssetToLoad);
	ABCHECK(LoadedAssetPath.IsValid());

	GetMesh()->SetSkeletalMesh(LoadedAssetPath.Get());
	SetCharacterState(ECharacterState::READY);
}

void ANewCharacter::SetCharacterState(ECharacterState NewState)
{
	ABCHECK(CurrentState != NewState); // 굳이 왜 확인할까?
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::LOADING:
	{
		if (bIsPlayer)
		{
			DisableInput(NewPlayerController);

			NewPlayerController->GetHUDWidget()->BindCharacterStat(CharacterStat);

			//언리얼 5는 Pawn이 아니라 Pawn의 Controller에서 PlayerState를 가져와야만 한다.
			auto NewPlayerState = Cast<ANewPlayerState>(NewPlayerController->PlayerState);
			//auto NewPlayerState = Cast<ANewPlayerState>(PlayerState);

			ABCHECK(nullptr != NewPlayerState);
			CharacterStat->SetNewLevel(NewPlayerState->GetCharacterLevel());

			// 추가한 코드
			auto GameState = GetWorld()->GetGameState();
			auto NewGameStateBase = Cast<ANewGameStateBase>(GameState);
			
			NewGameStateBase->SetTotalScore(NewPlayerState->GetGameScore());
		}
		else
		{
			auto NewGameMode = Cast<ANewGameMode>(GetWorld()->GetAuthGameMode());
			ABCHECK(nullptr != NewGameMode);

			int32 TargetLevel = FMath::CeilToInt(((float)NewGameMode->GetScore() * 0.08f));
			YU_LOG_FORMAT(Error, TEXT("*** TargetLevel  : %f"), ((float)NewGameMode->GetScore() * 0.08f));


			int32 FinalLevel = FMath::Clamp<int32>(TargetLevel, 1, 20);
			YU_LOG_FORMAT(Error, TEXT("*** New NPC Level : %d"), FinalLevel);

			CharacterStat->SetNewLevel(FinalLevel);
		}

		SetActorHiddenInGame(true);
		HPBarWidget->SetHiddenInGame(true);
		SetCanBeDamaged(false);
		break;
	}

	case ECharacterState::READY:
	{
		if (bIsPlayer)
		{
			EnableInput(NewPlayerController);
			SetControlMode(ControlMode::DIABLO);
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		}
		else
		{
			SetControlMode(ControlMode::NPC);
			GetCharacterMovement()->MaxWalkSpeed = 300.0f;
			NewAIController->RunAI();
		}

		SetActorHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(false);
		SetCanBeDamaged(true);

		// CharacterStat의 OnHPIsZero Delegate 바인딩
		CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
			SetCharacterState(ECharacterState::DEAD);
			});
		
		// 위젯에 CharacterStat 바인딩
		auto CharacterWidget = Cast<UNewUserWidget>(HPBarWidget->GetUserWidgetObject());
		ABCHECK(nullptr != CharacterWidget);
		CharacterWidget->BindCharacterStat(CharacterStat);
		break;
	}

	case ECharacterState::DEAD:
	{
		SetActorEnableCollision(false);
		GetMesh()->SetHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(true);
		NewAnim->SetDeadAnim();
		SetCanBeDamaged(false);

		if (bIsPlayer)
		{
			DisableInput(NewPlayerController);

			// 죽었을 시 경험치, 레벨 초기화
			auto NewPlayerState = Cast<ANewPlayerState>(NewPlayerController->PlayerState);
			NewPlayerState->DeadPlayerData();
		}
		else
		{
			NewAIController->StopAI();
		}

		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
			if (bIsPlayer)
			{
				//NewPlayerController->RestartLevel();
				NewPlayerController->ShowResultUI();
			}
			else
			{
				Destroy();
			}
			}),
			DeadTimer, false);
		break;
	}
	}
}

ECharacterState ANewCharacter::GetCharacterState() const
{
	return CurrentState;
}

int32 ANewCharacter::GetExp() const
{
	return CharacterStat->GetDropExp();
}

float ANewCharacter::GetFinalAttackRange() const
{
	return (nullptr != CurrentWeapon) ? CurrentWeapon->GetAttackRange() : AttackRange;
}

float ANewCharacter::GetFinalAttackDamage() const
{
	float AttackDamage = (nullptr != CurrentWeapon) ?
		(CharacterStat->GetAttack() + CurrentWeapon->GetAttackDamage()) : //true
			CharacterStat->GetAttack();														//false

	float AttackModifier = (nullptr != CurrentWeapon) ?
		CurrentWeapon->GetAttackModifier() : 1.0f;

	return AttackDamage * AttackModifier;
}


void ANewCharacter::SetControlMode(ControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;

	switch (CurrentControlMode)
	{
	case ControlMode::GTA:
		ArmLength = 450.0f;
		// 기본적으로 부착된 컴포넌트는 상대적인 회전값을 가지는데, 0으로 초기화.
		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);

		// Pawn의 회전값을 곧바로 SpringArm에 사용
		SpringArm->bUsePawnControlRotation = true;

		// SpringArm의 상위객체에 의해 회전값이 설정될것인지?
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;

		// Pawn과 카메라 사이 장애물이 감지되면 줌 인할지?
		SpringArm->bDoCollisionTest = true;

		// SpringArm 시점에 따라 컨트롤러의 RotaionYaw가 바뀔지?
		bUseControllerRotationYaw = false;

		// Pawn이 움직이는 방향으로 Pawn 자동회전
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		// 위의 회전속도를 지정
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 150.0f, 0.0f);
		break;

	case ControlMode::DIABLO:
		// 멀리서 보도록 설정
		//SpringArm->TargetArmLength = 800.0f;

		// 기본적으로 부착된 컴포넌트는 상대적인 회전값을 가지는데, 
		// 부착된 컴포넌트로부터 45도 내려보도록 설정
		//SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));

		ArmLength = 800.0f;
		ArmRotation = FRotator(-45.0f, 0.0f, 0.0f);


		// Pawn의 회전값을 SpringArm에 사용하지 않음. 회전값은 항상 고정.
		SpringArm->bUsePawnControlRotation = false;

		// SpringArm의 상위객체에 의해 회전값이 설정될것인지? 회전값은 항상 고정.
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;

		// Pawn과 카메라 사이 장애물이 감지되면 줌 인할지? 안함
		SpringArm->bDoCollisionTest = false;

		// 카메라의 회전에 따라서 RotaionYaw가 바뀔지? 함
		// Pawn의 회전을 자유롭게 둘지, 그렇지 않고 고정할지를 의미함 
		bUseControllerRotationYaw = false; //해당값을 fasle로 두고, 아래값을 true로 두면 45도 기준 움직임이 자연스럽게 바뀜
		GetCharacterMovement()->bUseControllerDesiredRotation = true;

		// Pawn이 움직이는 방향으로 Pawn 자동회전 안함 - 그래서 속도도 필요없다.
		GetCharacterMovement()->bOrientRotationToMovement = false;
		break;

	case ControlMode::NPC:
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		break;
	}
}

void ANewCharacter::ViewChange()
{
	switch (CurrentControlMode)
	{
	case ControlMode::GTA:
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(ControlMode::DIABLO);
		break;
	case ControlMode::DIABLO:
		GetController()->SetControlRotation(SpringArm->GetRelativeRotation());
		SetControlMode(ControlMode::GTA);
		break;
	}
}

void ANewCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void ANewCharacter::AttackEndComboState()
{
	CanNextCombo = false;
	IsComboInputOn = false;
	CurrentCombo = 0;
}

//공격 당하는 입장
float ANewCharacter::TakeDamage(float DamageAmount,
	FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	YU_LOG_FORMAT(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	CharacterStat->SetDamage(FinalDamage); //공격받음

	// 추가한 구문
	if (CurrentState == ECharacterState::DEAD)
	{
		if (EventInstigator->IsPlayerController())
		{
			auto aNewPlayerController = Cast<ANewPlayerController>(EventInstigator);
			ABCHECK(nullptr != aNewPlayerController, 0.0f);
			aNewPlayerController->NPCKill(this);
		}
	}
	return FinalDamage;
}

void ANewCharacter::AttackCheck()
{
	float FinalAttackRange = GetFinalAttackRange();

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * FinalAttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(50.0f),
		Params);

	//드로우 디버그
#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * FinalAttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = FinalAttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime
	);

	// 비주얼로거
	UE_VLOG_LOCATION(this, Untitled, Verbose, GetActorLocation(), 50.0f, FColor::Blue, TEXT("Attack Position"));
	UE_VLOG_CAPSULE(this, Untitled, Verbose, GetActorLocation() - GetActorForwardVector()*AttackRadius,
		HalfHeight, AttackRadius, CapsuleRot, DrawColor, TEXT("Attack Area"));


#endif
	if (bResult)
	{
		if (HitResult.GetActor())
		{
			YU_LOG_FORMAT(Warning, TEXT("Hit Actor Name : %s"), *HitResult.GetActor()->GetName());

			// 데미지 프레임워크
			FDamageEvent  DamageEvent;
			HitResult.GetActor()->TakeDamage(GetFinalAttackDamage(), DamageEvent, GetController(), this);
		}
	}
}

bool ANewCharacter::CanSetWeapon()
{
	//return (nullptr == CurrentWeapon);
	return true;
}

void ANewCharacter::SetWeapon(ANewWeapon* NewWeapon)
{
	ABCHECK(nullptr != NewWeapon);

	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	FName WeaponSocket(TEXT("hand_rSocket"));
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}

void ANewCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsPlayerControlled())
	{
		SetControlMode(ControlMode::DIABLO);
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else
	{
		SetControlMode(ControlMode::NPC);
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
}

