#include "CoopGameGameModeBase.h"

#include "AI/CoopGameTrackerBot.h"
#include "Components/CoopGameHealthComponent.h"
#include "CoopGameCharacter.h"
#include "CoopGameGameState.h"
#include "CoopGamePlayerState.h"
#include "EngineUtils.h"

ACoopGameGameModeBase::ACoopGameGameModeBase()
{
	PrimaryActorTick.TickInterval = 1.0f;
	PrimaryActorTick.bCanEverTick = true;

	GameStateClass = ACoopGameGameState::StaticClass();
	PlayerStateClass = ACoopGamePlayerState::StaticClass();
}

bool ACoopGameGameModeBase::IsAnyPlayerAliveInTheGame() const
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
			return true;
		}
	}

	return false;
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

	SetupGameState(ECoopGameState::SpawningBotsInProgress);
}

void ACoopGameGameModeBase::FinishSpawningBots()
{
	GetWorldTimerManager().ClearTimer(SpawnNewBotTimer);
}

void ACoopGameGameModeBase::PrepareForTheNextSpawningWave()
{
	GetWorldTimerManager().SetTimer(NextSpawningWaveTimer, this, &ThisClass::StartSpawningBots, DelayBetweenSpawningWaves);

	SetupGameState(ECoopGameState::WaitingSpawningBotsStart);

	RestartDeadPlayers();
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
	if (IsAnyPlayerAliveInTheGame())
	{
		return;
	}

	GameOver();
}

void ACoopGameGameModeBase::GameOver()
{
	FinishSpawningBots();

	SetupGameState(ECoopGameState::GameOver);

	// @TODO: Finish up the match. Prepent to players 'Game Over'.
	UE_LOG(LogTemp, Log, TEXT("GAME OVER"));
}

void ACoopGameGameModeBase::SetupGameState(ECoopGameState NewGameState)
{
	ACoopGameGameState* GS = GetGameState<ACoopGameGameState>();
	if (GS)
	{
		GS->SetGameState(NewGameState);
	}
}

void ACoopGameGameModeBase::RestartDeadPlayers()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (PlayerController && PlayerController->GetPawn() == nullptr)
		{
			RestartPlayer(PlayerController);
		}
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
