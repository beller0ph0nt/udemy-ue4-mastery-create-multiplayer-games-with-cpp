#include "CoopGameGameState.h"

#include "Net/UnrealNetwork.h"

void ACoopGameGameState::OnRep_GameState(EGameState OldGameState)
{
	OnGameStateChange(GameState, OldGameState);
}

void ACoopGameGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACoopGameGameState, GameState);
}