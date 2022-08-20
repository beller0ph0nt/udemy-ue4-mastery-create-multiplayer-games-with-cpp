// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StealthGameExtractionZone.generated.h"

class UBoxComponent;

UCLASS()
class STEALTHGAME_API AStealthGameExtractionZone : public AActor
{
	GENERATED_BODY()
	
public:	
	AStealthGameExtractionZone();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* BoxComponent;

	UFUNCTION()
	void BoxComponentBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
