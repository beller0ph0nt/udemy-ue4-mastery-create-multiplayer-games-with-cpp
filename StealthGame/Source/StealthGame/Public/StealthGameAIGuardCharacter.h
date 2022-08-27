// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StealthGameAIGuardCharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class STEALTHGAME_API AStealthGameAIGuardCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AStealthGameAIGuardCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	UFUNCTION()
	void OnHearNoise(APawn* NoiseInstigator, const FVector& Location, float Volume);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComponent;
};
