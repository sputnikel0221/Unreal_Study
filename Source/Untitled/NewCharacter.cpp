// Fill out your copyright notice in the Description page of Project Settings.


#include "NewCharacter.h"

// Sets default values
ANewCharacter::ANewCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	// Capsule�� RootComponent, Get���� �����´�.
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	// Capsule�� ���� ������ ���� �ʴ´� ����?

	// �Ʒ��� �������� GetMesh() �ܿ� �� ����.
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_PAWNBOARD(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	if (SK_PAWNBOARD.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_PAWNBOARD.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);	// BP����

	// BP�κ��� Animation Instance Ŭ������ ã�Ƽ� ������
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

	// ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !1 ! ! ! ! ! ����? 
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


	// ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !1 ! ! ! ! ! ����? 
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

		// �⺻������ ������ ������Ʈ�� ������� ȸ������ �����µ�, 0���� �ʱ�ȭ.
		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);

		// Pawn�� ȸ������ ��ٷ� SpringArm�� ���
		SpringArm->bUsePawnControlRotation = true;

		// SpringArm�� ������ü�� ���� ȸ������ �����ɰ�����?
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;

		// Pawn�� ī�޶� ���� ��ֹ��� �����Ǹ� �� ������?
		SpringArm->bDoCollisionTest = true;

		// SpringArm ������ ���� ��Ʈ�ѷ��� RotaionYaw�� �ٲ���?
		bUseControllerRotationYaw = false;

		// Pawn�� �����̴� �������� Pawn �ڵ�ȸ��
		GetCharacterMovement()->bOrientRotationToMovement = true;
		// ���� ȸ���ӵ��� ����
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 150.0f, 0.0f);
	}
	else if (ControlMode == "Diablo")
	{
		// �ָ��� ������ ����
		SpringArm->TargetArmLength = 800.0f;
		
		// �⺻������ ������ ������Ʈ�� ������� ȸ������ �����µ�, 
		// ������ ������Ʈ�κ��� 45�� ���������� ����
		SpringArm->SetRelativeRotation(FRotator(-45.0f,0.0f,0.0f));

		// Pawn�� ȸ������ SpringArm�� ������� ����. ȸ������ �׻� ����.
		SpringArm->bUsePawnControlRotation = false;

		// SpringArm�� ������ü�� ���� ȸ������ �����ɰ�����? ȸ������ �׻� ����.
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;

		// Pawn�� ī�޶� ���� ��ֹ��� �����Ǹ� �� ������? ����
		SpringArm->bDoCollisionTest = false;

		// ī�޶��� ȸ���� ���� RotaionYaw�� �ٲ���? ��
		// Pawn�� ȸ���� �����Ӱ� ����, �׷��� �ʰ� ���������� �ǹ���
		bUseControllerRotationYaw = true;

		// Pawn�� �����̴� �������� Pawn �ڵ�ȸ�� ���� - �׷��� �ӵ��� �ʿ����.
		GetCharacterMovement()->bOrientRotationToMovement = false;

	}
}

