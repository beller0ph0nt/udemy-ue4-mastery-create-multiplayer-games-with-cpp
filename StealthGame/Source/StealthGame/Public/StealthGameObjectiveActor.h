// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StealthGameObjectiveActor.generated.h"

class UParticleSystem;
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class STEALTHGAME_API AStealthGameObjectiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStealthGameObjectiveActor();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* CollisionSphereComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* PickupEffect;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void PlayEffects();
};
