#include "Fountain.h"

// Sets default values
AFountain::AFountain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//생성자에 추가한 코드
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));
	Water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WATER"));

	// LightSplash
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("LIGHT"));
    Splash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SPLASH"));

	//반드시 루트컴포넌트를 지정
	RootComponent = Body;
	Water->SetupAttachment(Body);

	//LightSplash 상속
	Light->SetupAttachment(Body);
	Splash->SetupAttachment(Body);

	//Water의 상대적위치를 135위로 설정
	Water->SetRelativeLocation(FVector(0.0f, 0.0f, 135.0f));
	//LightSplash 위치
	Light->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));
	Splash->SetRelativeLocation(FVector(0.0f, 0.0f, 195.0f));
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
