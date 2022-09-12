// Fill out your copyright notice in the Description page of Project Settings.

#include "CoopGameCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ACoopGameCharacter::ACoopGameCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ACoopGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACoopGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForwardOrBackward"), this, &ThisClass::MoveForwardOrBackward);
	PlayerInputComponent->BindAxis(TEXT("MoveLeftOrRight"), this, &ThisClass::MoveLeftOrRight);

	PlayerInputComponent->BindAxis(TEXT("TurnUpOrDown"), this, &ThisClass::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("TurnLeftOrRight"), this, &ThisClass::AddControllerYawInput);
}

void ACoopGameCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACoopGameCharacter::MoveForwardOrBackward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void ACoopGameCharacter::MoveLeftOrRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}
