// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CoopGameWeapon.generated.h"

class UDamageType;
class UCameraShakeBase;
class UParticleSystem;
class USkeletalMeshComponent;

USTRUCT()
struct FHitSync
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FVector_NetQuantize ImpactPoint;

	UPROPERTY()
	FVector_NetQuantizeNormal ImpactNormal;

	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> PhysicalSurface;

};

UCLASS()
class COOPGAME_API ACoopGameWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ACoopGameWeapon();

	void StartFire();
	void EndFire();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerEndPointParameterName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* MuzzleEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* FleshImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShakeBase> FireCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float DelayBetweenShots = 0.5;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin = 0.0f))
	float BulletSpreadConeHalfAngleInDegrees = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BaseDamage = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float HeadShotDamageMultipier = 4.0f;

	UPROPERTY(ReplicatedUsing = OnRep_HitSync)
	FHitSync HitSync;

	virtual void Fire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();

	UFUNCTION()
	void OnRep_HitSync();

private:
	bool bIsFiring = false;
	FTimerHandle DelayBetweenShotsTimerHandle;

	void PlayFireEffects(const FVector& TracerEndPoint);
	void PlayImpactEffects(EPhysicalSurface PhysicalSurface, const FVector& ImpactPoint, const FVector& ImpactNormal);
	void FireHandler();
};
