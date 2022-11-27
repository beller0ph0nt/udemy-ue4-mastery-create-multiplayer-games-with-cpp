#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "CoopGameGameModeBase.generated.h"

enum class ECoopGameState : uint8;
class ACoopGameCharacter;

UCLASS()
class COOPGAME_API ACoopGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPostLoginSignature, APlayerController*, NewPlayer);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayerKillSignature, const AActor*, VictimActor, const AActor*, KillerActor);
public:
	FPostLoginSignature OnPostLogin;

	UPROPERTY(BlueprintAssignable, Category = "GameMode")
	FPlayerKillSignature OnPlayerKill;

	ACoopGameGameModeBase();
	bool IsAnyPlayerAliveInTheGame() const;
	ACoopGameCharacter* FindNearestPlayerTo(const AActor* Actor);

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void StartPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "GameMode|BotsSpawn")
	float SpawnNewBotRate = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode|BotsSpawn")
	int32 BotsPerWaveMultiplyer = 2;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode|BotsSpawn")
	float DelayBetweenSpawningWaves = 13.0f;

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnNewBot();

	void StartSpawningBots();
	void FinishSpawningBots();
	void PrepareForTheNextSpawningWave();
	bool IsAllBotsHaveDied();
	void CheckSpawningBotsState();
	void CheckAnyPlayerAlive();
	void GameOver();

private:
	FTimerHandle SpawnNewBotTimer;
	FTimerHandle NextSpawningWaveTimer;

	int32 CurrentWaveNumber = 0;
	int32 BotsToSpawnPerCurrentWave = 1;

	void SetupGameState(ECoopGameState NewGameState);
	void RestartDeadPlayers();

	UFUNCTION()
	void SpawnNewBotHandler();
};
