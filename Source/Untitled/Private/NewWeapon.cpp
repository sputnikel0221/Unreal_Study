// Fill out your copyright notice in the Description page of Project Settings.


#include "NewWeapon.h"

// Sets default values
ANewWeapon::ANewWeapon()
{
	PrimaryActorTick.bCanEverTick = false; //���� tick�� �ʿ� �����Ƿ� false�� ����

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>RockyHammer(TEXT("/Script/Engine.SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blunt/Blunt_RockyHammer/SK_Blunt_RockyHammer.SK_Blunt_RockyHammer'"));
	if (RockyHammer.Succeeded())
	{
		Weapon->SetSkeletalMesh(RockyHammer.Object);
	}
	Weapon->SetCollisionProfileName(TEXT("NoCollision")); //����� Collison����

	AttackRange = 150.0f; // ���� �⺻ ��Ÿ�

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

