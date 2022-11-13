#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "CoopGameGameModeBase.generated.h"


UCLASS()
class COOPGAME_API ACoopGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPostLoginSignature, APlayerController*, NewPlayer);
	FPostLoginSignature OnPostLogin;

	ACoopGameGameModeBase();

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

private:
	FTimerHandle SpawnNewBotTimer;
	FTimerHandle NextSpawningWaveTimer;

	int32 CurrentWaveNumber = 0;
	int32 BotsToSpawnPerCurrentWave = 1;

	UFUNCTION()
	void SpawnNewBotHandler();
};
