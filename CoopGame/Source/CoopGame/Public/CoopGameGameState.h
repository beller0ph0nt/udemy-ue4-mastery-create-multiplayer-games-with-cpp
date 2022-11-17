#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"

#include "CoopGameGameState.generated.h"

UENUM(BlueprintType)
enum class ECoopGameState : uint8
{
	WaitingSpawningBotsStart ,
	SpawningBotsInProgress,
	GameOver
};

UCLASS()
class COOPGAME_API ACoopGameGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ACoopGameGameState();

	void SetGameState(ECoopGameState NewGameState);

protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_GameState, Category = "GameState")
	ECoopGameState GameState;

	UFUNCTION(BlueprintImplementableEvent, Category = "GameState|Events")
	void OnGameStateChange(ECoopGameState NewGameState, ECoopGameState OldGameState);

private:
	UFUNCTION()
	void OnRep_GameState(ECoopGameState OldGameState);
};
