#include "CoopGameGameModeBase.h"

#include "AI/CoopGameTrackerBot.h"
#include "Components/CoopGameHealthComponent.h"
#include "EngineUtils.h"

ACoopGameGameModeBase::ACoopGameGameModeBase()
{
	PrimaryActorTick.TickInterval = 1.0f;
	PrimaryActorTick.bCanEverTick = true;
}

void ACoopGameGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	OnPostLogin.Broadcast(NewPlayer);
}

void ACoopGameGameModeBase::StartPlay()
{
	Super::StartPlay();

	PrepareForTheNextSpawningWave();
}

void ACoopGameGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckSpawningBotsState();
}

void ACoopGameGameModeBase::StartSpawningBots()
{
	GetWorldTimerManager().ClearTimer(NextSpawningWaveTimer);

	CurrentWaveNumber++;
	BotsToSpawnPerCurrentWave = BotsPerWaveMultiplyer * CurrentWaveNumber;

	GetWorldTimerManager().SetTimer(SpawnNewBotTimer, this, &ThisClass::SpawnNewBotHandler, SpawnNewBotRate, true, 0.0f);
}

void ACoopGameGameModeBase::FinishSpawningBots()
{
	GetWorldTimerManager().ClearTimer(SpawnNewBotTimer);
}

void ACoopGameGameModeBase::PrepareForTheNextSpawningWave()
{
	GetWorldTimerManager().SetTimer(NextSpawningWaveTimer, this, &ThisClass::StartSpawningBots, DelayBetweenSpawningWaves);
}

bool ACoopGameGameModeBase::IsAllBotsHaveDied()
{
	for (TActorIterator<ACoopGameTrackerBot> It(GetWorld()); It; ++It)
	{
		ACoopGameTrackerBot* Bot = *It;
		if (!Bot)
		{
			continue;
		}

		UCoopGameHealthComponent* HealthComponent = Cast<UCoopGameHealthComponent>(Bot->GetComponentByClass(UCoopGameHealthComponent::StaticClass()));
		if (HealthComponent && 0.0f < HealthComponent->GetHealth())
		{
			return false;
		}
	}

	return true;
}

void ACoopGameGameModeBase::CheckSpawningBotsState()
{
	bool bIsPreparingForTheNextSpawningWave = GetWorldTimerManager().IsTimerActive(NextSpawningWaveTimer);
	if (0 < BotsToSpawnPerCurrentWave || bIsPreparingForTheNextSpawningWave)
	{
		return;
	}

	if (IsAllBotsHaveDied())
	{
		PrepareForTheNextSpawningWave();
	}
}

void ACoopGameGameModeBase::SpawnNewBotHandler()
{
	SpawnNewBot();

	BotsToSpawnPerCurrentWave--;
	if (BotsToSpawnPerCurrentWave <= 0)
	{
		FinishSpawningBots();
	}
}
