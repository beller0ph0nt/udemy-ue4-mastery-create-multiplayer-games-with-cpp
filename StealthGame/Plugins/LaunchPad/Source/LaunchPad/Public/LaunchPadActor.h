// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaunchPadActor.generated.h"

class UBoxComponent;
class UParticleSystem;
class UStaticMeshComponent;

UCLASS()
class LAUNCHPAD_API ALaunchPadActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ALaunchPadActor();

private:
	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere, Category = "Components")
	UBoxComponent* CollisionBoxComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = "Launch Pad")
	double LaunchPitchAngle = 45.0;

	UPROPERTY(EditAnywhere, Category = "Launch Pad")
	double LaunchStrength = 1000.0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Launch Pad")
	UParticleSystem* LaunchEffect;
};
