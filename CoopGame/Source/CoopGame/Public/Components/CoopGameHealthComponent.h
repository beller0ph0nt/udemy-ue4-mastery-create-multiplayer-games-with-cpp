// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoopGameHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPGAME_API UCoopGameHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCoopGameHealthComponent();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "HealthComponent")
	float Health = 100.0f;

	virtual void BeginPlay() override;
};
