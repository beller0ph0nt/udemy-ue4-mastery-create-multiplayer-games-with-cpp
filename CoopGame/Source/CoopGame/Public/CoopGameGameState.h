#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "CoopGameGameState.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
	WaitingSpawningBotsStart,
	SpawningBotsInProgress,
	WaitingSpawningBotsComplete,
	GameOver
};

UCLASS()
class COOPGAME_API ACoopGameGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Replicated = OnRep_GameState, Category = "GameState")
	EGameState GameState;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "GameState|Events")
	void OnGameStateChange(EGameState NewGameState, EGameState OldGameState);

private:
	UFUNCTION()
	void OnRep_GameState(EGameState OldGameState);
};
