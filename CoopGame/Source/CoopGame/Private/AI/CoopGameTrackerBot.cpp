#include "AI/CoopGameTrackerBot.h"

#include "Components/CoopGameHealthComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoopGameCharacter.h"
#include "CoopGameGameModeBase.h"
#include "CoopGameGameState.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"

ACoopGameTrackerBot::ACoopGameTrackerBot()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCanEverAffectNavigation(false);
	MeshComponent->SetSimulatePhysics(true);
	RootComponent = MeshComponent;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetSphereRadius(ExplosionRadius);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SphereComponent->SetupAttachment(RootComponent);

	HealthComponent = CreateDefaultSubobject<UCoopGameHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChanged);

	SetReplicates(true);
}

void ACoopGameTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() == ROLE_Authority && bAtLeastOnePlayerInTheGame && !bIsExploded)
	{
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
}

void ACoopGameTrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (bSelfDestructionStarted)
	{
		return;
	}

	ACoopGameCharacter* PlayerPawn = Cast<ACoopGameCharacter>(OtherActor);
	if (PlayerPawn)
	{
		bSelfDestructionStarted = true;
		OnRep_bSelfDestructionStarted();

		if (GetLocalRole() == ROLE_Authority)
		{
			GetWorldTimerManager().SetTimer(SelfDestructTimerHandler,
				[this]() { UGameplayStatics::ApplyDamage(this, 20.0f, GetInstigatorController(), this, nullptr); },
				SelfDestructTimerRate, true, 0.0f);
		}
	}
}

void ACoopGameTrackerBot::BeginPlay()
{
	Super::BeginPlay();

	if (IsNetMode(NM_ListenServer) || IsNetMode(NM_Standalone))
	{
		HandleNewPlayer(UGameplayStatics::GetPlayerController(this, 0));
	}
	else
	{
		ACoopGameGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ACoopGameGameModeBase>();
		if (GameMode)
		{
			GameMode->OnPostLogin.AddDynamic(this, &ThisClass::HandleNewPlayer);
		}
	}

	MaterialInstanceDynamic = MeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComponent->GetMaterial(0));
}

FVector ACoopGameTrackerBot::GetNextPathPoint()
{
	// TODO: Just for testing purposes. Will not work in multiplayer.
	ACoopGameCharacter* Character = Cast<ACoopGameCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	check(Character);

	UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), Character);
	check(NavigationPath);

	return 1 < NavigationPath->PathPoints.Num() ? NavigationPath->PathPoints[1] : GetActorLocation();
}

void ACoopGameTrackerBot::SelfExplode()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (bIsExploded)
		{
			return;
		}

		bIsExploded = true;
		OnRep_bIsExploded();

		GetWorldTimerManager().ClearTimer(SelfDestructTimerHandler);

		TArray<AActor*> IgnoreActors{ this };
		UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoreActors, this, GetInstigatorController(), true);

		SetLifeSpan(ClientsSynchronizationTimeSpanBeforeBotDestroy);
	}
}

void ACoopGameTrackerBot::OnHealthChanged(UCoopGameHealthComponent* OwnerHealthComponent, float Health, float Damage)
{
	if (MaterialInstanceDynamic)
	{
		MaterialInstanceDynamic->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
	}

	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Bot health %s"), *FString::SanitizeFloat(Health)));

	if (Health <= 0.0f)
	{
		SelfExplode();
	}
}

void ACoopGameTrackerBot::HandleNewPlayer(APlayerController* NewPlayer)
{
	if (bAtLeastOnePlayerInTheGame)
	{
		return;
	}

	bAtLeastOnePlayerInTheGame = true;
	NextPathPoint = GetNextPathPoint();
}

void ACoopGameTrackerBot::OnRep_bSelfDestructionStarted()
{
	if (!IsNetMode(NM_DedicatedServer) && bSelfDestructionStarted)
	{
		UGameplayStatics::SpawnSoundAttached(StartSelfDestructSound, RootComponent);

		UKismetSystemLibrary::PrintString(this, TEXT("Play self destruct sound"));
	}
}

void ACoopGameTrackerBot::OnRep_bIsExploded()
{
	if (!IsNetMode(NM_DedicatedServer) && bIsExploded)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
		DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 12, FColor::Red, false, 6.0f, 0, 1.0f);

		MeshComponent->SetVisibility(false);
		MeshComponent->SetPhysicsAngularVelocityInDegrees(FVector::Zero());
		MeshComponent->SetPhysicsLinearVelocity(FVector::Zero());

		UKismetSystemLibrary::PrintString(this, TEXT("Explosion"));
	}
}

void ACoopGameTrackerBot::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACoopGameTrackerBot, bIsExploded);
}
