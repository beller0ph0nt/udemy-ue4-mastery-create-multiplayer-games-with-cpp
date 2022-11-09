#include "PickupActor.h"

#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"

APickupActor::APickupActor()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetSphereRadius(DecalSize);
	RootComponent = SphereComponent;

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->AddRelativeRotation(FRotator(90.0, 0.0, 0.0));
	DecalComponent->DecalSize = FVector(DecalThickness, DecalSize, DecalSize);
	DecalComponent->SetupAttachment(SphereComponent);
}

void APickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// @TODO: Grant a powerup to player if available
}

void APickupActor::BeginPlay()
{
	Super::BeginPlay();	
}
