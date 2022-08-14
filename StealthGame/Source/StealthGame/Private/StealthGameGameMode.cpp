// Copyright Epic Games, Inc. All Rights Reserved.

#include "StealthGameGameMode.h"
#include "StealthGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AStealthGameGameMode::AStealthGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
