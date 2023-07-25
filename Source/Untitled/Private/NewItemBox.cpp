// Fill out your copyright notice in the Description page of Project Settings.


#include "NewItemBox.h"
#include "NewWeapon.h"
#include "NewCharacter.h"

// Sets default values
ANewItemBox::ANewItemBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Script/Engine.StaticMesh'/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	if (SM_BOX.Succeeded())
	{
		Box->SetStaticMesh(SM_BOX.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN(TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
	if (P_CHESTOPEN.Succeeded())
	{
		Effect->SetTemplate(P_CHESTOPEN.Object);
		Effect->bAutoActivate = false;
	}

	Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	
	// TEXT로 Collision채널 설정
	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));


	WeaponItemClass = ANewWeapon::StaticClass();
}

// Called when the game starts or when spawned
void ANewItemBox::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void ANewItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANewItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ANewItemBox::OnCharacterOverlap);
}

void ANewItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	YU_LOG(Warning);

	auto NewCharacter = Cast<ANewCharacter>(OtherActor);
	if (nullptr != NewCharacter && nullptr != WeaponItemClass)
	{
		if (NewCharacter->CanSetWeapon())
		{
			auto NewWeapon = GetWorld()->SpawnActor<ANewWeapon>(WeaponItemClass,
				FVector::ZeroVector, FRotator::ZeroRotator);
			NewCharacter->SetWeapon(NewWeapon);

			Effect->Activate(true);
			Box->SetHiddenInGame(true, true);
			SetActorEnableCollision(false);
			Effect->OnSystemFinished.AddDynamic(this, &ANewItemBox::OnEffectFinished);
		}
		else
		{
			YU_LOG_FORMAT(Warning, TEXT("Already Equipped Weapon"));
		}
	}
	else
	{
		YU_LOG_FORMAT(Warning, TEXT("Weapon ItemClass is NULL"));
	}
}

void ANewItemBox::OnEffectFinished(UParticleSystemComponent* PSystem)
{
	Destroy();
}

