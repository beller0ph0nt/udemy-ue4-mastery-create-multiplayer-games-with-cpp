// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGameAIGuardCharacter.h"

#include "DrawDebugHelpers.h"
#include "Engine/TargetPoint.h"
#include "Net/UnrealNetwork.h"
#include "Perception/PawnSensingComponent.h"
#include "StealthGameGameMode.h"

AStealthGameAIGuardCharacter::AStealthGameAIGuardCharacter()
{
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ThisClass::OnSeePawn);
	PawnSensingComponent->OnHearNoise.AddDynamic(this, &ThisClass::OnHearNoise);
}

void AStealthGameAIGuardCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AStealthGameAIGuardCharacter, State);
}

void AStealthGameAIGuardCharacter::SetState(EAIGuardState NewState)
{
	if (State == NewState || State == EAIGuardState::Alerted)
	{
		return;
	}

	State = NewState;
	OnRep_State();
}

void AStealthGameAIGuardCharacter::StopMovement()
{
	auto SelfController = GetController();
	if (SelfController)
	{
		SelfController->StopMovement();
	}
}

void AStealthGameAIGuardCharacter::MoveToTheCurrentPatrolPoint()
{
	if (!bIsPatrol || PatrolPoints.Num() <= 0)
	{
		return;
	}

	OnMoveToTheCurrentPatrolPoint(PatrolPoints[CurrentPatrolPointIndex]);
}

void AStealthGameAIGuardCharacter::UpdateCurrentPatrolPoint()
{
	if (!bIsPatrol || PatrolPoints.Num() < 2)
	{
		return;
	}

	int NewIndex = CurrentPatrolPointIndex + PatrolPointIndexDiff;
	if (0 <= NewIndex && NewIndex < PatrolPoints.Num())
	{
		CurrentPatrolPointIndex = NewIndex;
	}
	else
	{
		PatrolPointIndexDiff *= -1;
		UpdateCurrentPatrolPoint();
	}
}

void AStealthGameAIGuardCharacter::RotateGuardToTheNoiseDirection(const FVector& NoiseLocation)
{
	FVector NewDirection = NoiseLocation - GetActorLocation();
	NewDirection.Normalize();

	FRotator NewRotation = FRotationMatrix::MakeFromX(NewDirection).Rotator();
	NewRotation.Roll = GetActorRotation().Roll;
	NewRotation.Pitch = GetActorRotation().Pitch;

	SetActorRotation(NewRotation);
}

void AStealthGameAIGuardCharacter::OnMissionCompleted()
{
	GetWorld()->GetTimerManager().ClearTimer(RestoreOriginalRotationTimer);
	StopMovement();
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

	SetState(EAIGuardState::Alerted);
	StopMovement();
}

void AStealthGameAIGuardCharacter::OnHearNoise(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	DrawDebugSphere(GetWorld(), Location, 32.0f, 32, FColor::Red, false, 3.0f);

	RotateGuardToTheNoiseDirection(Location);

	GetWorld()->GetTimerManager().ClearTimer(RestoreOriginalRotationTimer);
	GetWorld()->GetTimerManager().SetTimer(RestoreOriginalRotationTimer, this, &ThisClass::OnRestoreOriginalRotation, 3.0f);

	SetState(EAIGuardState::Suspicious);
	StopMovement();
}

void AStealthGameAIGuardCharacter::OnRestoreOriginalRotation()
{
	SetActorRotation(OriginalRotation);
	SetState(EAIGuardState::Idle);
	MoveToTheCurrentPatrolPoint();
}

void AStealthGameAIGuardCharacter::OnRep_State()
{
	OnStateChanged(State);
}

void AStealthGameAIGuardCharacter::BeginPlay()
{
	Super::BeginPlay();

	auto GameMode = Cast<AStealthGameGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		GameMode->OnMissionCompletedEvent.AddUObject(this, &ThisClass::OnMissionCompleted);
	}

	OriginalRotation = GetActorRotation();
	SetState(EAIGuardState::Idle);
	MoveToTheCurrentPatrolPoint();
}

void AStealthGameAIGuardCharacter::OnPatrolPointReachedSuccessfully()
{
	UpdateCurrentPatrolPoint();
	MoveToTheCurrentPatrolPoint();
}
