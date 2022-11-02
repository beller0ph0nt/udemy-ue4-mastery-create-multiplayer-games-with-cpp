#include "AI/CoopGameTrackerBot.h"

#include "Components/StaticMeshComponent.h"

ACoopGameTrackerBot::ACoopGameTrackerBot()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCanEverAffectNavigation(false);
	RootComponent = MeshComponent;
}

void ACoopGameTrackerBot::BeginPlay()
{
	Super::BeginPlay();	
}

void ACoopGameTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
