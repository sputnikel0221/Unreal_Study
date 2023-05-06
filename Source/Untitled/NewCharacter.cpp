// Fill out your copyright notice in the Description page of Project Settings.


#include "NewCharacter.h"

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

	// 아래의 설정들은 GetMesh() 외에 다 같다.
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_PAWNBOARD(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	if (SK_PAWNBOARD.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_PAWNBOARD.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);	// BP버전

	// BP로부터 Animation Instance 클래스를 찾아서 가져옴
	static ConstructorHelpers::FClassFinder<UAnimInstance> NewAnimInstance(TEXT("/Script/Engine.AnimBlueprint'/Game/Book/BP_WarriorAnim.BP_WarriorAnim_C'"));
	{
		GetMesh()->SetAnimInstanceClass(NewAnimInstance.Class);
	}

	SetControlMode("Diablo");
}

// Called when the game starts or when spawned
void ANewCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ANewCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 입력으로 받은 X,Y값(Z는 위라서 제외)을 확인하고 맞게 이동
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
	AddMovementInput(DirectionToMove);
}

// Called to bind functionality to input
void ANewCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &ANewCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("RightLeft"), this, &ANewCharacter::RightLeft);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ANewCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ANewCharacter::Turn);
}

void ANewCharacter::UpDown(float UDValue)
{
	//AddMovementInput(GetActorForwardVector(), UDValue);
	//AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), UDValue);
	DirectionToMove.X = UDValue;
}

void ANewCharacter::RightLeft(float RLValue)
{
	//AddMovementInput(GetActorRightVector(), RLValue);
	//AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), RLValue);
	DirectionToMove.Y = RLValue;
}

void ANewCharacter::LookUp(float aValue)
{
	AddControllerPitchInput(aValue);
}

void ANewCharacter::Turn(float aValue)
{
	AddControllerYawInput(aValue);
}

void ANewCharacter::SetControlMode(FString ControlMode)
{
	if (ControlMode == "GTA")
	{

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
		// 위의 회전속도를 지정
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 150.0f, 0.0f);
	}
	else if (ControlMode == "Diablo")
	{
		// 멀리서 보도록 설정
		SpringArm->TargetArmLength = 800.0f;
		
		// 기본적으로 부착된 컴포넌트는 상대적인 회전값을 가지는데, 
		// 부착된 컴포넌트로부터 45도 내려보도록 설정
		SpringArm->SetRelativeRotation(FRotator(-45.0f,0.0f,0.0f));

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

	}
}

