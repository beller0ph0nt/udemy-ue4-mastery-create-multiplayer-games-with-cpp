// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StealthGameGameMode.generated.h"

UCLASS(minimalapi)
class AStealthGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE(FOnMissionCompletedDelegate)

public:
	FOnMissionCompletedDelegate OnMissionCompletedEvent;

	AStealthGameGameMode();

	void CompleteMission(APawn* InstigatorPawn, bool bIsMissionSucceeded);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ViewTargets")
	TSubclassOf<AActor> MissionCompleteViewTarget;

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void OnMissionCompleted(APawn* InstigatorPawn, bool bIsMissionSucceeded);
};



