// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "Untitled.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Pawn.h"
#include "NewPawn.generated.h"

UCLASS()
class UNTITLED_API ANewPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ANewPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnyWhere, Category = Collision)
		UCapsuleComponent* Capsule;
	UPROPERTY(VisibleAnyWhere, Category = Visual)
		USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleAnyWhere, Category = Movement)
		UFloatingPawnMovement* Movement;
	UPROPERTY(VisibleAnyWhere, Category = Camera)
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnyWhere, Category = Camera)
		UCameraComponent* Camera;


private:
	void UpDown(float UDValue);
	void RightLeft(float RLValue);
};
