#include "CoopGamePlayerState.h"

#include "Net/UnrealNetwork.h"

void ACoopGamePlayerState::AddScore(float ScoreDelta)
{
	PlayerScore += ScoreDelta;
}

void ACoopGamePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACoopGamePlayerState, PlayerScore);
}
