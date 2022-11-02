// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoopGameHealthComponent.generated.h"

class AController;
class UDamageType;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChangedSignature,
	UCoopGameHealthComponent*, OwnerHealthComponent,
	float, Health,
	float, Damage);

USTRUCT()
struct FHealthComponentSync
{
	GENERATED_BODY()

public:
	UPROPERTY()
	float Health;

	UPROPERTY()
	float Damage;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPGAME_API UCoopGameHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "HealthComponent|Events")
	FOnHealthChangedSignature OnHealthChanged;

	UCoopGameHealthComponent();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_HealthComponentSync)
	FHealthComponentSync HealthComponentSync;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HealthComponent")
	float DefaultHealth = 100.0f;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_HealthComponentSync();

private:
	UFUNCTION()
	void TakeAnyDamageHandler(
		AActor* DamagedActor,
		float Damage,
		const UDamageType* DamageType,
		AController* InstigatedBy,
		AActor* DamageCauser);
};
