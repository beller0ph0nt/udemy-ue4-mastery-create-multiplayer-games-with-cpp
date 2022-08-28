// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGameAIGuardCharacter.h"

#include "DrawDebugHelpers.h"
#include "Perception/PawnSensingComponent.h"
#include "StealthGameGameMode.h"

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

	DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 32.0f, 32, FColor::White, false, 3.0f);

	auto GameMode = Cast<AStealthGameGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->CompleteMission(Pawn, false);
	}
}

void AStealthGameAIGuardCharacter::OnHearNoise(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	DrawDebugSphere(GetWorld(), Location, 32.0f, 32, FColor::Red, false, 3.0f);

	FVector NewDirection = Location - GetActorLocation();
	NewDirection.Normalize();

	FRotator NewRotation = FRotationMatrix::MakeFromX(NewDirection).Rotator();
	NewRotation.Roll = GetActorRotation().Roll;
	NewRotation.Pitch = GetActorRotation().Pitch;

	SetActorRotation(NewRotation);

	GetWorld()->GetTimerManager().ClearTimer(RestoreOriginalRotationTimer);
	GetWorld()->GetTimerManager().SetTimer(RestoreOriginalRotationTimer, this, &ThisClass::OnRestoreOriginalRotation, 3.0f);
}

void AStealthGameAIGuardCharacter::OnRestoreOriginalRotation()
{
	SetActorRotation(OriginalRotation);
}

void AStealthGameAIGuardCharacter::BeginPlay()
{
	Super::BeginPlay();
	OriginalRotation = GetActorRotation();
}
