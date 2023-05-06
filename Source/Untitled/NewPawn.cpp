// Fill out your copyright notice in the Description page of Project Settings.


#include "NewPawn.h"

// Sets default values
ANewPawn::ANewPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	// 계층관계
	RootComponent = Capsule;
	Mesh->SetupAttachment(RootComponent);
	SpringArm->SetupAttachment(RootComponent);
	// Movement는 부착될 수 없다
	Camera->SetupAttachment(SpringArm); // SpringArm이라는 셀카봉에 고정

	// Capsule의 크기 설정
	Capsule->SetCapsuleHalfHeight(88.0f);
	Capsule->SetCapsuleRadius(34.0f);
	// 작업환경이 다른 에셋의 세부항목 조절
	Mesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	// 길이 400cm에 Y축으로 -15도 회전된 셀카봉 사용
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_PAWNBOARD(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	if (SK_PAWNBOARD.Succeeded()) {
		Mesh->SetSkeletalMesh(SK_PAWNBOARD.Object);
	}

	Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);	// BP버전

	// BP로부터 Animation Instance 클래스를 찾아서 가져옴
	static ConstructorHelpers::FClassFinder<UAnimInstance> NewAnimInstance(TEXT("/Script/Engine.AnimBlueprint'/Game/Book/BP_WarriorAnim.BP_WarriorAnim_C'"));
	{
		Mesh->SetAnimInstanceClass(NewAnimInstance.Class);
	}
}

// Called when the game starts or when spawned
void ANewPawn::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void ANewPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANewPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &ANewPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("RightLeft"), this, &ANewPawn::RightLeft);
}

void ANewPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	YU_LOG_FORMAT(Warning, TEXT("Pawn"));
}

void ANewPawn::PossessedBy(AController* NewController)
{
	YU_LOG_FORMAT(Warning, TEXT("Pawn Possessed"));
	Super::PossessedBy(NewController);
}

void ANewPawn::UpDown(float UDValue)
{
	//YU_LOG_FORMAT(Warning, TEXT("%f"), UDValue);
	AddMovementInput(GetActorForwardVector(), UDValue);
}

void ANewPawn::RightLeft(float RLValue)
{
	//YU_LOG_FORMAT(Warning, TEXT("%f"), RLValue);
	AddMovementInput(GetActorRightVector(), RLValue);
}

