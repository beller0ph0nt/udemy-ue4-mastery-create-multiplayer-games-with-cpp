// Fill out your copyright notice in the Description page of Project Settings.


#include "StealthGameGameState.h"

#include "StealthGamePlayerController.h"

void AStealthGameGameState::MulticastCompleteMission_Implementation(APawn* InstigatorPawn, bool bIsMissionSucceeded)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AStealthGamePlayerController* CurrentPlayerController = Cast<AStealthGamePlayerController>(It->Get());
		if (CurrentPlayerController && CurrentPlayerController->IsLocalPlayerController())
		{
			CurrentPlayerController->OnMissionCompleted(InstigatorPawn, bIsMissionSucceeded);

			APawn* Pawn = CurrentPlayerController->GetPawn();
			if (Pawn)
			{
				Pawn->DisableInput(CurrentPlayerController);
			}
		}
	}
}
