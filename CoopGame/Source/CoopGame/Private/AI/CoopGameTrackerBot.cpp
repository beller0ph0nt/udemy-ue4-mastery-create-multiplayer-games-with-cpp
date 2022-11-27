#include "AI/CoopGameTrackerBot.h"

#include "Components/CoopGameHealthComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoopGameCharacter.h"
#include "CoopGameGameModeBase.h"
#include "CoopGameGameState.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"

static bool TrackerBotDebugDrawing = false;
static FAutoConsoleVariableRef CVAR_TrackerBotDebugDrawing(
	TEXT("COOP.TrackerBotDebugDrawing"),
	TrackerBotDebugDrawing,
	TEXT("Draw debug for tracker bot"),
	ECVF_Cheat);

ACoopGameTrackerBot::ACoopGameTrackerBot()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCanEverAffectNavigation(false);
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetCollisionObjectType(ECC_Pawn);
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

void ACoopGameTrackerBot::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(RefreshPathTimer);
	GetWorldTimerManager().ClearTimer(SelfDestructTimer);
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

			if (TrackerBotDebugDrawing)
			{
				DrawDebugString(GetWorld(), GetActorLocation(), TEXT("Target reached!!!"));
			}
		}
		else
		{
			FVector Force = NextPathPoint - GetActorLocation();
			Force.Normalize();
			Force *= MovementForce;

			MeshComponent->AddForce(Force, NAME_None, bAccelerationChange);

			if (TrackerBotDebugDrawing)
			{
				DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + Force, Force.Size(), FColor::Yellow, false, 0.0f);
			}
		}

		if (TrackerBotDebugDrawing)
		{
			DrawDebugSphere(GetWorld(), NextPathPoint, 20.0f, 12, FColor::Yellow, false, 0.0f);
		}
	}
}

void ACoopGameTrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	ACoopGameTrackerBot* OtherTrackerBot = Cast<ACoopGameTrackerBot>(OtherActor);
	if (OtherTrackerBot)
	{
		BotsCounter++;
		UpdateDamagePowerLevelBoost();
	}

	if (bSelfDestructionStarted)
	{
		return;
	}

	ACoopGameCharacter* PlayerPawn = Cast<ACoopGameCharacter>(OtherActor);
	if (PlayerPawn && !UCoopGameHealthComponent::IsFriends(this, PlayerPawn))
	{
		bSelfDestructionStarted = true;
		OnRep_bSelfDestructionStarted();

		if (GetLocalRole() == ROLE_Authority)
		{
			GetWorldTimerManager().SetTimer(SelfDestructTimer,
				[this]()
				{
					UE_LOG(LogTemp, Log, TEXT("TrackerBot: %s. Selfdestruct timer run"), *GetName());
					UGameplayStatics::ApplyDamage(this, 20.0f, GetInstigatorController(), this, nullptr);
				},
				SelfDestructTimerRate, true, 0.0f);
		}
	}
}

void ACoopGameTrackerBot::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	ACoopGameTrackerBot* OtherTrackerBot = Cast<ACoopGameTrackerBot>(OtherActor);
	if (OtherTrackerBot)
	{
		BotsCounter--;
		UpdateDamagePowerLevelBoost();
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
			bAtLeastOnePlayerInTheGame = GameMode->IsAnyPlayerAliveInTheGame();
			if (bAtLeastOnePlayerInTheGame)
			{
				NextPathPoint = GetNextPathPoint();
			}
			//GameMode->OnPostLogin.AddDynamic(this, &ThisClass::HandleNewPlayer);
		}
	}

	MaterialInstanceDynamic = MeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComponent->GetMaterial(0));
}

FVector ACoopGameTrackerBot::GetNextPathPoint()
{
	using UHealth = UCoopGameHealthComponent;

	if (GetLocalRole() == ROLE_Authority)
	{
		ACoopGameCharacter* NearestCharacter = nullptr;

		ACoopGameGameModeBase* GM = Cast<ACoopGameGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			NearestCharacter = GM->FindNearestPlayerTo(this);
		}

		//check(NearestCharacter);
		if (NearestCharacter)
		{
			UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), NearestCharacter);
			//check(NavigationPath);

			GetWorldTimerManager().ClearTimer(RefreshPathTimer);
			GetWorldTimerManager().SetTimer(RefreshPathTimer,
				[this]() { NextPathPoint = GetNextPathPoint(); },
				RefreshPathDelay,
				false);

			return (NavigationPath && 1 < NavigationPath->PathPoints.Num()) ? NavigationPath->PathPoints[1] : GetActorLocation();
		}
	}

	return {};
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

		UE_LOG(LogTemp, Log, TEXT("TrackerBot: %s. Clear timers"), *GetName());
		GetWorldTimerManager().ClearTimer(RefreshPathTimer);
		GetWorldTimerManager().ClearTimer(SelfDestructTimer);

		TArray<AActor*> IgnoreActors{ this };
		UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage + DamageBoost, GetActorLocation(), ExplosionRadius, nullptr, IgnoreActors, this, GetInstigatorController(), true);

		SetLifeSpan(ClientsSynchronizationTimeSpanBeforeBotDestroy);
	}
}

void ACoopGameTrackerBot::UpdateDamagePowerLevelBoost()
{
	DamageBoost = FMath::Clamp(BotsCounter * DamageBoostPerBot, 0.0f, MaxDamageBoost);
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("DamageBoost = %f"), DamageBoost));

	if (MaterialInstanceDynamic)
	{
		MaterialInstanceDynamic->SetScalarParameterValue("DamageBoostAlpha", DamageBoost / MaxDamageBoost);
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

		if (TrackerBotDebugDrawing)
		{
			DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 12, FColor::Red, false, 6.0f, 0, 1.0f);
		}

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
