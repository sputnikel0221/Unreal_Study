// Fill out your copyright notice in the Description page of Project Settings.


#include "NewWeapon.h"

// Sets default values
ANewWeapon::ANewWeapon()
{
	PrimaryActorTick.bCanEverTick = false; //굳이 tick할 필요 없으므로 false로 설정

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>RockyHammer(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blunt/Blunt_RockyHammer/SK_Blunt_RockyHammer.SK_Blunt_RockyHammer'"));
	if (RockyHammer.Succeeded())
	{
		Weapon->SetSkeletalMesh(RockyHammer.Object);
	}
	Weapon->SetCollisionProfileName(TEXT("NoCollision")); //무기는 Collison없음

	AttackRange = 150.0f; // 무기 기본 사거리

	AttackDamageMin = 1.0f;
	AttackDamageMax = 10.0f;
	AttackModifierMin = 1.0f;
	AttackModifierMax = 1.25f;
}

// Called when the game starts or when spawned
void ANewWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	AttackDamage = FMath::RandRange(AttackDamageMin, AttackDamageMax);
	AttackModifier = FMath::RandRange(AttackModifierMin, AttackModifierMax);

	YU_LOG_FORMAT(Warning, TEXT("*** You Got Weapon : Damage : %f / Modifier : %f"), AttackDamage, AttackModifier);
}

// Called every frame
void ANewWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ANewWeapon::GetAttackRange() const
{
	return 	AttackRange;
}

float ANewWeapon::GetAttackDamage() const
{
	return AttackDamage;
}

float ANewWeapon::GetAttackModifier() const
{
	return AttackModifier;
}

