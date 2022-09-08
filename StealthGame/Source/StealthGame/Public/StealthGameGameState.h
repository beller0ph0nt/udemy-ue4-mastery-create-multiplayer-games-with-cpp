// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "StealthGameGameState.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHGAME_API AStealthGameGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastCompleteMission(APawn* InstigatorPawn, bool bIsMissionSucceeded);
};
