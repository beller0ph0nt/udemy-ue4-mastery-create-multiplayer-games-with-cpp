#include "CoopGameGameModeBase.h"

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

void ACoopGameGameModeBase::StartSpawningBots()
{
	CurrentWaveNumber++;
	BotsToSpawnPerCurrentWave = BotsPerWaveMultiplyer * CurrentWaveNumber;

	GetWorldTimerManager().SetTimer(SpawnNewBotTimer, this, &ThisClass::SpawnNewBotHandler, SpawnNewBotRate, true, 0.0f);
}

void ACoopGameGameModeBase::FinishSpawningBots()
{
	GetWorldTimerManager().ClearTimer(SpawnNewBotTimer);
	PrepareForTheNextSpawningWave();
}

void ACoopGameGameModeBase::PrepareForTheNextSpawningWave()
{
	FTimerHandle NextSpawningWaveTimer;
	GetWorldTimerManager().SetTimer(NextSpawningWaveTimer, this, &ThisClass::StartSpawningBots, DelayBetweenSpawningWaves);
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
