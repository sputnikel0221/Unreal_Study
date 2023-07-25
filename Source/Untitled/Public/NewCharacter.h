// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Untitled.h"
#include "GameFramework/Character.h"
#include "NewCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class UNTITLED_API ANewCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANewCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual float TakeDamage(float DamageAmount,
		FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, Category = UI)
		class UWidgetComponent* HPBarWidget;


	UPROPERTY(VisibleAnywhere, Category = Stat)
		class UNewCharacterStatComponent* CharacterStat;

private:
	FVector DirectionToMove = FVector::ZeroVector;
	void UpDown(float UDValue);
	void RightLeft(float RLValue);

	void LookUp(float aValue);
	void Turn(float aValue);

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;

	UPROPERTY()
		class UNewAnimInstance* NewAnim;


protected:
	enum class ControlMode
	{
		GTA,
		DIABLO,
		NPC
	};

	void SetControlMode(ControlMode NewControlMode);
	ControlMode CurrentControlMode = ControlMode::GTA; //일단 기본모드로 설정

	void ViewChange();

	// 관련 변수를 추가, 오류를 방지해 항상 초기화를 해놓는 모습
	float ArmLength = 0.0f;
	FRotator ArmRotation = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.0f;
	float ArmRotationSpeed = 0.0f;

private:
	void AttackStartComboState();
	void AttackEndComboState();
	void AttackCheck(); //Collision 함수

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool CanNextCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsComboInputOn;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 MaxCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;

public:
	bool CanSetWeapon();
	void SetWeapon(class ANewWeapon* NewWeapon);
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		class ANewWeapon* CurrentWeapon;

	virtual void PossessedBy(AController* NewController) override;

	void Attack();
	FOnAttackEndDelegate OnAttackEnd;

private:
	void OnAssetLoadCompleted();

	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;


public:
	void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const;

private:
	int32 AssetIndex = 0;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		ECharacterState CurrentState;
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
		bool bIsPlayer;

	UPROPERTY()
		class ANewPlayerController* NewPlayerController;
	UPROPERTY()
		class ANewAIController* NewAIController;

	// 추가
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
		float DeadTimer;
	FTimerHandle DeadTimerHandle = {};

public:
	int32 GetExp() const;
	
	public:
		float GetFinalAttackRange() const;

		float GetFinalAttackDamage() const;
};
