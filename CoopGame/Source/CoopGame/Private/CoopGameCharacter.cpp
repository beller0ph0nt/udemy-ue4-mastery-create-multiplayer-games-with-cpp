#include "CoopGameCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CoopGameHealthComponent.h"
#include "CoopGame.h"
#include "CoopGameWeapon.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

ACoopGameCharacter::ACoopGameCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<UCoopGameHealthComponent>(TEXT("HealthComponent"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(TRACE_CHANNEL_WEAPON, ECollisionResponse::ECR_Ignore);

	WeaponSocketName = "WeaponSocket";
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

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ThisClass::StartFire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ThisClass::EndFire);
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

	HealthComponent->OnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChanged);

	if (GetLocalRole() == ROLE_Authority)
	{
		FActorSpawnParameters Parameters;
		Parameters.Owner = this;
		Parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		CurrentWeapon = GetWorld()->SpawnActor<ACoopGameWeapon>(StarterWeapon, FVector::ZeroVector, FRotator::ZeroRotator, Parameters);
		if (CurrentWeapon)
		{
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
		}
	}
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

void ACoopGameCharacter::StartFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}
}

void ACoopGameCharacter::EndFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->EndFire();
	}
}

void ACoopGameCharacter::OnHealthChanged(
	UCoopGameHealthComponent* Component,
	float Health,
	float Damage,
	const UDamageType* DamageType,
	AController* InstigatedBy,
	AActor* DamageCauser)
{
	if (Health <= 0.0f && !bIsDied)
	{
		bIsDied = true;
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.0f);
	}
}

void ACoopGameCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACoopGameCharacter, CurrentWeapon);
	DOREPLIFETIME(ACoopGameCharacter, bIsDied);
}