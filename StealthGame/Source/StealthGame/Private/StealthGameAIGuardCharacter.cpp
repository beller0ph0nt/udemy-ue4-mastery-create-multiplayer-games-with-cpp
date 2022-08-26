// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGameAIGuardCharacter.h"

#include "Perception/PawnSensingComponent.h"

AStealthGameAIGuardCharacter::AStealthGameAIGuardCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
}

void AStealthGameAIGuardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AStealthGameAIGuardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStealthGameAIGuardCharacter::BeginPlay()
{
	Super::BeginPlay();
}
