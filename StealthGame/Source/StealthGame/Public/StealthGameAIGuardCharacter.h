// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StealthGameAIGuardCharacter.generated.h"

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

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	EAIGuardState GetState() const;
	
	UFUNCTION()
	void SetState(EAIGuardState NewState);

private:
	FRotator OriginalRotation;
	FTimerHandle RestoreOriginalRotationTimer;
	EAIGuardState State;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	UFUNCTION()
	void OnHearNoise(APawn* NoiseInstigator, const FVector& Location, float Volume);

	UFUNCTION()
	void OnRestoreOriginalRotation();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIGuardState NewState);
};
