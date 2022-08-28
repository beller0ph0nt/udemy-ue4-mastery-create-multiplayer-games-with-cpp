// Copyright Epic Games, Inc. All Rights Reserved.

#include "StealthGameGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "StealthGameCharacter.h"
#include "StealthGameHUD.h"
#include "UObject/ConstructorHelpers.h"

AStealthGameGameMode::AStealthGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	static ConstructorHelpers::FClassFinder<AHUD> HUDClassFinder(TEXT("/Game/Blueprints/BP_StealthGameHUD"));
	HUDClass = HUDClassFinder.Class;
}

void AStealthGameGameMode::CompleteMission(APawn* InstigatorPawn, bool bIsMissionSucceeded)
{
	if (InstigatorPawn)
	{
		InstigatorPawn->DisableInput(nullptr);

		AActor* Actor = UGameplayStatics::GetActorOfClass(this, MissionCompleteViewTarget);
		if (Actor)
		{ 
			auto PlayerController = Cast<APlayerController>(InstigatorPawn->GetController());
			if (PlayerController)
			{
				PlayerController->SetViewTargetWithBlend(Actor, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
			}
		}
	}

	OnMissionCompleted(InstigatorPawn, bIsMissionSucceeded);
}
