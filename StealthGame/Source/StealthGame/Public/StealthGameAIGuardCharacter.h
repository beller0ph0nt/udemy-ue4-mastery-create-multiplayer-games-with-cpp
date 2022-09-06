// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StealthGameAIGuardCharacter.generated.h"

class ATargetPoint;
class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EAIGuardState : uint8
{
	Suspicious = 0,
	Alerted,
	Idle
};

UCLASS()
class STEALTHGAME_API AStealthGameAIGuardCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AStealthGameAIGuardCharacter();
	EAIGuardState GetState() const { return State; }

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	FRotator OriginalRotation;
	FTimerHandle RestoreOriginalRotationTimer;
	int CurrentPatrolPointIndex = 0;
	int PatrolPointIndexDiff = 1;

	UPROPERTY(ReplicatedUsing = OnRep_State)
	EAIGuardState State;

	void SetState(EAIGuardState NewState);
	void StopMovement();
	void MoveToTheCurrentPatrolPoint();
	void UpdateCurrentPatrolPoint();
	void RotateGuardToTheNoiseDirection(const FVector& NoiseLocation);
	void OnMissionCompleted();

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	UFUNCTION()
	void OnHearNoise(APawn* NoiseInstigator, const FVector& Location, float Volume);

	UFUNCTION()
	void OnRestoreOriginalRotation();

	UFUNCTION()
	void OnRep_State();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

	UPROPERTY(EditInstanceOnly, Category = "AI")
	bool bIsPatrol = false;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bIsPatrol"))
	TArray<ATargetPoint*> PatrolPoints;

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIGuardState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnMoveToTheCurrentPatrolPoint(const ATargetPoint* CurrentPatrolPoint);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void OnPatrolPointReachedSuccessfully();
};
