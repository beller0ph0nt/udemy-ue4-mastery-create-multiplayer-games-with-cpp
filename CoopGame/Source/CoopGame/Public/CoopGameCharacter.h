// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "CoopGameCharacter.generated.h"

class ACoopGameWeapon;
class UCameraComponent;
class UCoopGameHealthComponent;
class USpringArmComponent;

UCLASS()
class COOPGAME_API ACoopGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACoopGameCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCoopGameHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<ACoopGameWeapon> StarterWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	bool bIsDied = false;

	virtual void BeginPlay() override;

private:
	bool bWantsToZoom = false;
	float DefaultFieldOfView = 0.0f;

	UPROPERTY(Replicated)
	ACoopGameWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float ZoomedFieldOfView = 65.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1f, ClampMax = 100.0f))
	float ZoomSpeed = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	FName WeaponSocketName;

	void MoveForwardOrBackward(float AxisValue);
	void MoveLeftOrRight(float AxisValue);

	void BeginCrouch();
	void EndCrouch();

	void BeginZoom() { bWantsToZoom = true; }
	void EndZoom() { bWantsToZoom = false; }

	void StartFire();
	void EndFire();

	UFUNCTION()
	void OnHealthChanged(
		UCoopGameHealthComponent* Component,
		float Health,
		float Damage,
		const UDamageType* DamageType,
		AController* InstigatedBy,
		AActor* DamageCauser);
};
