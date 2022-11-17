#include "CoopGameGameState.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

ACoopGameGameState::ACoopGameGameState()
{
	SetReplicates(true);
}

void ACoopGameGameState::SetGameState(ECoopGameState NewGameState)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		ECoopGameState OldGameState = GameState;
		GameState = NewGameState;
		OnRep_GameState(OldGameState);
	}
}

void ACoopGameGameState::OnRep_GameState(ECoopGameState OldGameState)
{
	if (IsNetMode(ENetMode::NM_DedicatedServer))
	{
		return;
	}

	OnGameStateChange(GameState, OldGameState);

	UKismetSystemLibrary::PrintString(this, TEXT("ACoopGameGameState::OnRep_GameState"));
}

void ACoopGameGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACoopGameGameState, GameState);
}