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

	//Trigger�� Delegate����
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ANewSection::OnTriggerBeginOverlap);

	//SM_GATE
	// 1 - Asset�� �ҷ���
	FString GateAssetPath = TEXT("'/Game/Book/StaticMesh/SM_GATE.SM_GATE'");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GATE(*GateAssetPath);
	if (!SM_GATE.Succeeded()) YU_LOG_FORMAT(Warning, TEXT("No SM_GATE"));

	// 2 - �����̸��� TEXT �迭�� �����.
	static FName GateSockets[] = { {TEXT("+XGate")}, {TEXT("-XGate")}, {TEXT("+YGate")}, {TEXT("-YGate")} };

	// 3 - TEXT �迭���� ���ϸ��� StaticMeshConponent�� �����,
	// �ش� ������Ʈ�� Root�� �� ���ϸ��� ��ġ�� ������Ų��.
	// GetMeshes��� StaticMesh�� ���� �迭�� �ش� ������Ʈ�� Add�Ѵ�.
	for (FName GateSocket : GateSockets)
	{
		ABCHECK(Mesh->DoesSocketExist(GateSocket));

		UStaticMeshComponent* NewGate = CreateDefaultSubobject<UStaticMeshComponent>(*GateSocket.ToString()); //ToString�̶� *����
		NewGate->SetStaticMesh(SM_GATE.Object);

		NewGate->SetupAttachment(RootComponent, GateSocket);
		NewGate->SetRelativeLocation(FVector(0.0f, -80.5f, 0.0f));
		GateMeshes.Add(NewGate);

		// Box������Ʈ ���� �� �ʱ�ȭ
		UBoxComponent* NewGateTrigger
			= CreateDefaultSubobject<UBoxComponent>(*GateSocket.ToString().Append(TEXT("Trigger")));

		NewGateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		NewGateTrigger->SetupAttachment(RootComponent, GateSocket);
		NewGateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
		NewGateTrigger->SetCollisionProfileName(TEXT("NewTrigger"));
		GateTriggers.Add(NewGateTrigger);

		//GateTrigger�� Delegate����
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


// ���¿� ���� ( 1-ū �ڽ� / 2-�� �ڽ��� / 3-�� �Ž� ȸ�� )
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

		// ��Timer�� Delegate ���ε�
		// NPC -> OnNPCSpawn
		GetWorld()->GetTimerManager().SetTimer(
			SpawnNPCTimerHandle, 
			FTimerDelegate::CreateUObject(this, &ANewSection::OnNPCSpawn), 
			EnemySpawnTime, 
			false
		);

		// ItemBox -> Lambda��
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
	CurrentState = NewState; //���� State����
}

// �� �Ž� ȸ��
void ANewSection::OperateGates(bool bOpen)
{
	for (UStaticMeshComponent* Gate : GateMeshes)
	{
		Gate->SetRelativeRotation(bOpen ? FRotator(0.0f, -90.0f, 0.0f) : FRotator::ZeroRotator);
	}
}


// �����ͷ� �ش� ������ �������� �ٲٰų� �̵��� �ߵ��ϴ� �Լ�
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

	// 1 - OverlappedComponent��� ���� ��ģ ��-Box�� �±װ� ù��°�� �����´�
	FName ComponentTag = OverlappedComponent->ComponentTags[0];

	// 2 - �ش� �±װ� +XGate���, ������ 2���ڸ� ���� +X�� ���� - �׷� ������ Mesh�������Ѵ�
	FName SocketName = FName(*ComponentTag.ToString().Left(2));
	if (!Mesh->DoesSocketExist(SocketName)) return;

	// 3 - �ش� ������ ��ġ�� ���ͺ����� �ִ´�
	FVector NewLocation = Mesh->GetSocketLocation(SocketName);

	// 4 - OverlapMultiByObjectType �Լ��� �Ű��������� �����, ������ ��ġ�� � ������Ʈ�� �����ϴ��� Ȯ���Ѵ�
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

	// 5- �ش� ����� ���ٸ�, ������ �����Ѵ�
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

	// ���� �߰�
	auto NewGameMode = Cast<ANewGameMode>(GetWorld()->GetAuthGameMode());
	ABCHECK(nullptr != NewGameMode);
	NewGameMode->AddScore(NewPlayerController); //�ش� �÷��̾��� ������ ��ü������ �ø�

	SetState(ESectionState::COMPLETE);
}

