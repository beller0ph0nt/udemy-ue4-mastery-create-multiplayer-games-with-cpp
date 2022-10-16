// Fill out your copyright notice in the Description page of Project Settings.

#include "CoopGameCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ACoopGameCharacter::ACoopGameCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ACoopGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFieldOfView = bWantsToZoom ? ZoomedFieldOfView : DefaultFieldOfView;
	float NewFieldOfView = FMath::FInterpTo(CameraComponent->FieldOfView, TargetFieldOfView, DeltaTime, ZoomSpeed);
	CameraComponent->SetFieldOfView(NewFieldOfView);
}

void ACoopGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForwardOrBackward"), this, &ThisClass::MoveForwardOrBackward);
	PlayerInputComponent->BindAxis(TEXT("MoveLeftOrRight"), this, &ThisClass::MoveLeftOrRight);

	PlayerInputComponent->BindAxis(TEXT("TurnUpOrDown"), this, &ThisClass::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("TurnLeftOrRight"), this, &ThisClass::AddControllerYawInput);

	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &ThisClass::BeginCrouch);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &ThisClass::EndCrouch);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ThisClass::Jump);

	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &ThisClass::BeginZoom);
	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, this, &ThisClass::EndZoom);
}

void ACoopGameCharacter::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	if (CameraComponent)
	{
		OutLocation = CameraComponent->GetComponentLocation();
		OutRotation = CameraComponent->GetComponentRotation();
		return;
	}

	Super::GetActorEyesViewPoint(OutLocation, OutRotation);
}

void ACoopGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultFieldOfView = CameraComponent->FieldOfView;
}

void ACoopGameCharacter::MoveForwardOrBackward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void ACoopGameCharacter::MoveLeftOrRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void ACoopGameCharacter::BeginCrouch()
{
	Crouch();
}

void ACoopGameCharacter::EndCrouch()
{
	UnCrouch();
}
