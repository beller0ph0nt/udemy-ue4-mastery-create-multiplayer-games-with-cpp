// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CoopGameCharacter.generated.h"

class UCameraComponent;

UCLASS()
class COOPGAME_API ACoopGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACoopGameCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	virtual void BeginPlay() override;

private:
	void MoveForwardOrBackward(float AxisValue);
	void MoveLeftOrRight(float AxisValue);
};
