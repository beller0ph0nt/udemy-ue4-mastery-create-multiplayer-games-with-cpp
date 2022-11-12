#include "PickupActor.h"

#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "PowerupActor.h"

APickupActor::APickupActor()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetSphereRadius(DecalSize);
	RootComponent = SphereComponent;

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->AddRelativeRotation(FRotator(90.0, 0.0, 0.0));
	DecalComponent->DecalSize = FVector(DecalThickness, DecalSize, DecalSize);
	DecalComponent->SetupAttachment(SphereComponent);

	SetReplicates(true);
}

void APickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (GetLocalRole() == ROLE_Authority)
	{
		if (PowerupInstance)
		{
			PowerupInstance->PowerupActivate(OtherActor);
			PowerupInstance = nullptr;

			GetWorldTimerManager().SetTimer(RespawnPowerupTimer, this, &ThisClass::RespawnPowerup, RespawnPowerupInterval);
		}
	}
}

void APickupActor::BeginPlay()
{
	Super::BeginPlay();

	RespawnPowerup();
}

void APickupActor::RespawnPowerup()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (!PowerupClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("PowerupClass is nullptr in %s. Please update your blueprint."), *GetName());
			return;
		}

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		PowerupInstance = GetWorld()->SpawnActor<APowerupActor>(PowerupClass, GetTransform(), SpawnParameters);
	}
}
