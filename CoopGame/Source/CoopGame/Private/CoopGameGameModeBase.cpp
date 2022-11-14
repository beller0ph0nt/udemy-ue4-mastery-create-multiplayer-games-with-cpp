#include "CoopGameGameModeBase.h"

#include "AI/CoopGameTrackerBot.h"
#include "Components/CoopGameHealthComponent.h"
#include "CoopGameCharacter.h"
#include "CoopGameGameState.h"
#include "EngineUtils.h"

ACoopGameGameModeBase::ACoopGameGameModeBase()
{
	PrimaryActorTick.TickInterval = 1.0f;
	PrimaryActorTick.bCanEverTick = true;

	GameStateClass = ACoopGameGameState::StaticClass();
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
	CheckAnyPlayerAlive();
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
		const ACoopGameTrackerBot* Bot = *It;
		if (!Bot)
		{
			continue;
		}

		const UCoopGameHealthComponent* HealthComponent = Cast<UCoopGameHealthComponent>(Bot->GetComponentByClass(UCoopGameHealthComponent::StaticClass()));
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

void ACoopGameGameModeBase::CheckAnyPlayerAlive()
{
	for (TActorIterator<ACoopGameCharacter> It(GetWorld()); It; ++It)
	{
		const ACoopGameCharacter* Character = *It;
		if (!Character)
		{
			continue;
		}

		const UCoopGameHealthComponent* HealthComponent = Cast<UCoopGameHealthComponent>(Character->GetComponentByClass(UCoopGameHealthComponent::StaticClass()));
		if (HealthComponent && 0.0f < HealthComponent->GetHealth())
		{
			return;
		}
	}

	GameOver();
}

void ACoopGameGameModeBase::GameOver()
{
	FinishSpawningBots();

	// @TODO: Finish up the match. Prepent to players 'Game Over'.
	UE_LOG(LogTemp, Log, TEXT("GAME OVER"));
}

void ACoopGameGameModeBase::SetGameState(EGameState NewGameState)
{
	ACoopGameGameState* GS = GetGameState<ACoopGameGameState>();
	if (ensureAlways(GS))
	{
		GS->GameState = NewGameState;
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
