// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackHoleProjectileActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class BLACKHOLEPROJECTILE_API ABlackHoleProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABlackHoleProjectileActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Black Hole Projectile")
	FTimespan TimeUntilExplosionBegin = FTimespan(0, 0, 0, 3);

	UPROPERTY(EditAnywhere, Category = "Black Hole Projectile")
	FTimespan ExplosionDuration = FTimespan(0, 0, 0, 13);

	UPROPERTY(EditAnywhere, Category = "Black Hole Projectile")
	float GravityForceStrength = -1000000.0f;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	USphereComponent* AnnihilationSphere;

	UPROPERTY(EditAnywhere, Category = "Components")
	USphereComponent* GravitySphere;

private:
	void OnExplosionBegin();
	void OnExplosionEnd();

	UFUNCTION()
	void OnAnnihilationSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	bool bIsExplosionBegin = false;

	UPROPERTY()
	FTimerHandle TimerHandler;
};
