#include "AI/CoopGameTrackerBot.h"

#include "Components/StaticMeshComponent.h"
#include "CoopGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"

ACoopGameTrackerBot::ACoopGameTrackerBot()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCanEverAffectNavigation(false);
	RootComponent = MeshComponent;
}

void ACoopGameTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACoopGameTrackerBot::BeginPlay()
{
	Super::BeginPlay();	
}

FVector ACoopGameTrackerBot::GetNextPathPoint()
{
	// TODO: Just for testing purposes. Will not work in multiplayer.
	ACoopGameCharacter* Character = Cast<ACoopGameCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), Character);
	check(NavigationPath);

	return 1 < NavigationPath->PathPoints.Num() ? NavigationPath->PathPoints[0] : GetActorLocation();
}
