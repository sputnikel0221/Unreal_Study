// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Untitled.h"
#include "GameFramework/Character.h"
#include "NewCharacter.generated.h"

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

	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;

private:
	void UpDown(float UDValue);
	void RightLeft(float RLValue);

	void LookUp(float aValue);
	void Turn(float aValue);

	FVector DirectionToMove = FVector::ZeroVector;
	

protected:
	enum class ControlMode
	{
		GTA,
		DIABLO
	};

	void SetControlMode(ControlMode NewControlMode);
	ControlMode CurrentControlMode = ControlMode::GTA; //일단 기본모드로 설정

	void ViewChange();
};
