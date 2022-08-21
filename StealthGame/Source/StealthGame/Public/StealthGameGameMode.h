// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StealthGameGameMode.generated.h"

UCLASS(minimalapi)
class AStealthGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AStealthGameGameMode();

	void CompleteMission(APawn* InstigatorPawn);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ViewTargets")
	TSubclassOf<AActor> MissionCompleteViewTarget;

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void OnMissionCompleted(APawn* InstigatorPawn);
};



