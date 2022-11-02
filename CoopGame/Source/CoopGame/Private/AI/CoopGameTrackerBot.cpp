#include "AI/CoopGameTrackerBot.h"

#include "Components/StaticMeshComponent.h"
#include "CoopGameCharacter.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"

ACoopGameTrackerBot::ACoopGameTrackerBot()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCanEverAffectNavigation(false);
	MeshComponent->SetSimulatePhysics(true);
	RootComponent = MeshComponent;
}

void ACoopGameTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DistanceToTarget = (NextPathPoint - GetActorLocation()).Size();
	if (DistanceToTarget <= RequiredDistanceToTarget)
	{
		NextPathPoint = GetNextPathPoint();

		DrawDebugString(GetWorld(), GetActorLocation(), TEXT("Target reached!!!"));
	}
	else
	{
		FVector Force = NextPathPoint - GetActorLocation();
		Force.Normalize();
		Force *= MovementForce;

		MeshComponent->AddForce(Force, NAME_None, bAccelerationChange);

		DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + Force, Force.Size(), FColor::Yellow, false, 0.0f);
	}

	DrawDebugSphere(GetWorld(), NextPathPoint, 20.0f, 12, FColor::Yellow, false, 0.0f);
}

void ACoopGameTrackerBot::BeginPlay()
{
	Super::BeginPlay();

	NextPathPoint = GetNextPathPoint();
}

FVector ACoopGameTrackerBot::GetNextPathPoint()
{
	// TODO: Just for testing purposes. Will not work in multiplayer.
	ACoopGameCharacter* Character = Cast<ACoopGameCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), Character);
	check(NavigationPath);

	return 1 < NavigationPath->PathPoints.Num() ? NavigationPath->PathPoints[1] : GetActorLocation();
}
