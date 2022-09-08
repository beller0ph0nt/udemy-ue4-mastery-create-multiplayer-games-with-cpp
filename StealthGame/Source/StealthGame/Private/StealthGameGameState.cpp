// Fill out your copyright notice in the Description page of Project Settings.


#include "StealthGameGameState.h"

void AStealthGameGameState::MulticastCompleteMission_Implementation(APawn* InstigatorPawn, bool bIsMissionSucceeded)
{
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++)
	{
		APawn* Pawn = It->Get();
		if (Pawn && Pawn->IsLocallyControlled())
		{
			Pawn->DisableInput(nullptr);
		}
	}
}
