// Fill out your copyright notice in the Description page of Project Settings.


#include "NewPawn.h"

// Sets default values
ANewPawn::ANewPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

