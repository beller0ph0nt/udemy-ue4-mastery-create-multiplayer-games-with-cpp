// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGameAIGuardCharacter.h"

#include "DrawDebugHelpers.h"
#include "Perception/PawnSensingComponent.h"

AStealthGameAIGuardCharacter::AStealthGameAIGuardCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ThisClass::OnSeePawn);
	PawnSensingComponent->OnHearNoise.AddDynamic(this, &ThisClass::OnHearNoise);
}

void AStealthGameAIGuardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AStealthGameAIGuardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStealthGameAIGuardCharacter::OnSeePawn(APawn* Pawn)
{
	if (!Pawn)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 32.0f, 32, FColor::White, false, 10.0);
}

void AStealthGameAIGuardCharacter::OnHearNoise(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	DrawDebugSphere(GetWorld(), Location, 32.0f, 32, FColor::Red, false, 10.0);
}

void AStealthGameAIGuardCharacter::BeginPlay()
{
	Super::BeginPlay();
}
