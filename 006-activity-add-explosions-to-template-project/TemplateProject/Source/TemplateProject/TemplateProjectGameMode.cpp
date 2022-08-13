// Copyright Epic Games, Inc. All Rights Reserved.

#include "TemplateProjectGameMode.h"
#include "TemplateProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATemplateProjectGameMode::ATemplateProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
