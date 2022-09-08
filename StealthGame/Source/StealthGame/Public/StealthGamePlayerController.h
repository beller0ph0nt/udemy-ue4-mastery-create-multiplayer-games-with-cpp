// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "StealthGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STEALTHGAME_API AStealthGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "PlayerController")
	void OnMissionCompleted(APawn* InstigatorPawn, bool bIsMissionSucceeded);
};
