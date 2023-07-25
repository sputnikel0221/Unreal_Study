// Fill out your copyright notice in the Description page of Project Settings.


#include "NewSection.h"
#include "NewCharacter.h"
#include "NewItemBox.h"
#include "NewPlayerController.h"
#include "NewGameMode.h"

// Sets default values
ANewSection::ANewSection()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = Mesh;

	//SM_SQUARE
	FString AssetPath = TEXT("/Game/Book/StaticMesh/SM_SQUARE.SM_SQUARE");

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SQUARE(*AssetPath);

	if (SM_SQUARE.Succeeded())
	{
		Mesh->SetStaticMesh(SM_SQUARE.Object);
	}
	else
	{
		YU_LOG_FORMAT(Warning, TEXT("No SM_SQUARE"));
	}

	// Box / Trigger
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Trigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	Trigger->SetCollisionProfileName(TEXT("NewTrigger"));

	//Trigger의 Delegate연결
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ANewSection::OnTriggerBeginOverlap);

	//SM_GATE
	// 1 - Asset을 불러옴
	FString GateAssetPath = TEXT("'/Game/Book/StaticMesh/SM_GATE.SM_GATE'");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GATE(*GateAssetPath);
	if (!SM_GATE.Succeeded()) YU_LOG_FORMAT(Warning, TEXT("No SM_GATE"));

	// 2 - 소켓이름의 TEXT 배열을 만든다.
	static FName GateSockets[] = { {TEXT("+XGate")}, {TEXT("-XGate")}, {TEXT("+YGate")}, {TEXT("-YGate")} };

	// 3 - TEXT 배열마다 소켓명의 StaticMeshConponent를 만들고,
	// 해당 컴포넌트를 Root의 각 소켓명의 위치에 부착시킨다.
	// GetMeshes라는 StaticMesh를 넣을 배열에 해당 컴포넌트를 Add한다.
	for (FName GateSocket : GateSockets)
	{
		ABCHECK(Mesh->DoesSocketExist(GateSocket));

		UStaticMeshComponent* NewGate = CreateDefaultSubobject<UStaticMeshComponent>(*GateSocket.ToString()); //ToString이라서 *붙임
		NewGate->SetStaticMesh(SM_GATE.Object);

		NewGate->SetupAttachment(RootComponent, GateSocket);
		NewGate->SetRelativeLocation(FVector(0.0f, -80.5f, 0.0f));
		GateMeshes.Add(NewGate);

		// Box컴포넌트 생성 및 초기화
		UBoxComponent* NewGateTrigger
			= CreateDefaultSubobject<UBoxComponent>(*GateSocket.ToString().Append(TEXT("Trigger")));

		NewGateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		NewGateTrigger->SetupAttachment(RootComponent, GateSocket);
		NewGateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
		NewGateTrigger->SetCollisionProfileName(TEXT("NewTrigger"));
		GateTriggers.Add(NewGateTrigger);

		//GateTrigger의 Delegate연결
		NewGateTrigger->OnComponentBeginOverlap.AddDynamic(this, &ANewSection::OnGateTriggerBeginOverlap);
		NewGateTrigger->ComponentTags.Add(GateSocket);
	}

	bNoBattle = false;

	EnemySpawnTime = 2.0f;
	ItemBoxSpawnTime = 5.0f;
}

// Called when the game starts or when spawned
void ANewSection::BeginPlay()
{
	Super::BeginPlay();

	YU_LOG_FORMAT(Warning, TEXT("BeginPlay === READY ==="));
	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

// Called every frame
void ANewSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// 상태에 따른 ( 1-큰 박스 / 2-문 박스들 / 3-문 매시 회전 )
void ANewSection::SetState(ESectionState NewState)
{
	switch (NewState)
	{
	case ESectionState::READY:
	{
		YU_LOG_FORMAT(Warning, TEXT("=== READY ==="));
		Trigger->SetCollisionProfileName(TEXT("NewTrigger"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}
		OperateGates(true);
		break;
	}
	case ESectionState::BATTLE:
	{
		YU_LOG_FORMAT(Warning, TEXT("=== BATTLE ==="));
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}
		OperateGates(false);

		// 각Timer의 Delegate 바인딩
		// NPC -> OnNPCSpawn
		GetWorld()->GetTimerManager().SetTimer(
			SpawnNPCTimerHandle, 
			FTimerDelegate::CreateUObject(this, &ANewSection::OnNPCSpawn), 
			EnemySpawnTime, 
			false
		);

		// ItemBox -> Lambda식
		GetWorld()->GetTimerManager().SetTimer(
			SpawnItemBoxTimerHandle,
			FTimerDelegate::CreateLambda(
				[this]()->void {
				FVector2D RandXY = FMath::RandPointInCircle(600.0f);
				GetWorld()->SpawnActor<ANewItemBox>(GetActorLocation() +
				FVector(RandXY, 30.0f), FRotator::ZeroRotator);
				}),
			ItemBoxSpawnTime,
			false
		);

		break;
	}
	case ESectionState::COMPLETE:
	{
		YU_LOG_FORMAT(Warning, TEXT("=== COMPLETE ==="));
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("NewTrigger"));
		}
		OperateGates(true);
		break;
	}
	}
	CurrentState = NewState; //현재 State저장
}

// 문 매시 회전
void ANewSection::OperateGates(bool bOpen)
{
	for (UStaticMeshComponent* Gate : GateMeshes)
	{
		Gate->SetRelativeRotation(bOpen ? FRotator(0.0f, -90.0f, 0.0f) : FRotator::ZeroRotator);
	}
}


// 에디터로 해당 엑터의 변수값을 바꾸거나 이동시 발동하는 함수
void ANewSection::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//YU_LOG_FORMAT(Error, TEXT("OnConstruction READY"));
	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}


void ANewSection::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	YU_LOG_FORMAT(Error, TEXT("DELEGATE - TRIGGER"));
	if (CurrentState == ESectionState::READY)
	{
		SetState(ESectionState::BATTLE);
	}
}

void ANewSection::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	YU_LOG_FORMAT(Error, TEXT("DELEGATE - GATE - TRIGGER"));
	ABCHECK(OverlappedComponent->ComponentTags.Num() == 1);

	// 1 - OverlappedComponent라는 현재 겹친 문-Box의 태그값 첫번째를 가져온다
	FName ComponentTag = OverlappedComponent->ComponentTags[0];

	// 2 - 해당 태그가 +XGate라면, 왼쪽의 2글자만 떼서 +X로 본다 - 그런 소켓이 Mesh에존재한다
	FName SocketName = FName(*ComponentTag.ToString().Left(2));
	if (!Mesh->DoesSocketExist(SocketName)) return;

	// 3 - 해당 소켓의 위치를 벡터변수에 넣는다
	FVector NewLocation = Mesh->GetSocketLocation(SocketName);

	// 4 - OverlapMultiByObjectType 함수의 매개변수들을 만들고, 소켓의 위치에 어떤 오브젝트가 존재하는지 확인한다
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	FCollisionObjectQueryParams ObjectQueryParam(FCollisionObjectQueryParams::InitType::AllObjects);

	bool bResult = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		NewLocation,
		FQuat::Identity,
		ObjectQueryParam,
		FCollisionShape::MakeSphere(775.0f),
		CollisionQueryParam
	);

	// 5- 해당 결과가 없다면, 섹션을 생성한다
	if (!bResult)
	{
		auto NewSection = GetWorld()->SpawnActor<ANewSection>(NewLocation, FRotator::ZeroRotator);
	}
	else
	{
		YU_LOG_FORMAT(Warning, TEXT("NEW SECTION AREA is NOT Empty"));
	}
}

void ANewSection::OnNPCSpawn()
{
	auto KeyNPC = 
		GetWorld()->SpawnActor<ANewCharacter>(GetActorLocation() + FVector::UpVector + 88.0f, FRotator::ZeroRotator);
	ABCHECK(nullptr != KeyNPC);

	KeyNPC->OnDestroyed.AddDynamic(this, &ANewSection::OnKeyNPCDestroyed);
}

void ANewSection::OnKeyNPCDestroyed(AActor* DestroyedActor)
{
	auto NewCharacter = Cast<ANewCharacter>(DestroyedActor);
	ABCHECK(nullptr != NewCharacter);

	auto NewPlayerController = Cast<ANewPlayerController>(NewCharacter->LastHitBy);
	ABCHECK(nullptr != NewPlayerController);

	// 점수 추가
	auto NewGameMode = Cast<ANewGameMode>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != NewGameMode);
	NewGameMode->AddScore(NewPlayerController); //해당 플레이어의 점수와 전체점수를 올림

	SetState(ESectionState::COMPLETE);
}

