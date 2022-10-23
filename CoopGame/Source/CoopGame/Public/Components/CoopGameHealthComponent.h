// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoopGameHealthComponent.generated.h"

class AController;
class UDamageType;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature,
	UCoopGameHealthComponent*, HealthComponent,
	float, Health,
	float, Damage,
	const UDamageType*, DamageType,
	AController*, InstigatedBy,
	AActor*, DamageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPGAME_API UCoopGameHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "HealthComponent|Events")
	FOnHealthChangedSignature OnHealthChanged;

	UCoopGameHealthComponent();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "HealthComponent")
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = "HealthComponent")
	float DefaultHealth = 100.0f;

	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void TakeAnyDamageHandler(
		AActor* DamagedActor,
		float Damage,
		const UDamageType* DamageType,
		AController* InstigatedBy,
		AActor* DamageCauser);
};
