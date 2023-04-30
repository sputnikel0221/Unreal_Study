#include "Fountain.h"

// Sets default values
AFountain::AFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�����ڿ� �߰��� �ڵ�
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WATER"));

	// LightSplash
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("LIGHT"));
    Splash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SPLASH"));

	//�ݵ�� ��Ʈ������Ʈ�� ����
	RootComponent = Body;
	Water->SetupAttachment(Body);

	//LightSplash ���
	Light->SetupAttachment(Body);
	Splash->SetupAttachment(Body);

	//Water�� �������ġ�� 135���� ����
	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 135.0f));
	//LightSplash ��ġ
	Light->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));
	Splash->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_BODY(TEXT("/Script/Engine.StaticMesh'/"
			"Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/"
			"SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01'"));

	if (SM_BODY.Succeeded()) {
		Body->SetStaticMesh(SM_BODY.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_WATER(TEXT("/Script/Engine.StaticMesh'/Game/InfinityBladeGrassLands/Effects/FX_Meshes/Env/SM_Plains_Fountain_02.SM_Plains_Fountain_02'"));

	if (SM_WATER.Succeeded()) {
		Water->SetStaticMesh(SM_WATER.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> //UParticleSystem
		PS_SPLASH(TEXT("/Script/Engine.ParticleSystem'/Game/InfinityBladeGrassLands/Effects/FX_Ambient/Water/P_Water_Fountain_Splash_Base_01.P_Water_Fountain_Splash_Base_01'"));

	if (PS_SPLASH.Succeeded()) {
		Splash->SetTemplate(PS_SPLASH.Object); //SetTemplate
	}
}

// Called when the game starts or when spawned
void AFountain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
