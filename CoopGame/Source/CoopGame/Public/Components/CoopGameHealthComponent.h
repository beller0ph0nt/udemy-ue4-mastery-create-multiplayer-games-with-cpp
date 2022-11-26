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

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COOPGAME_API UCoopGameHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "HealthComponent|Events")
	FOnHealthChangedSignature OnHealthChanged;

	// TODO: Move this variable into separate component (TeamComponent) or
	// into character base class and access it through interface functions
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HealthComponent")
	uint8 TeamNumber = 255;

	UCoopGameHealthComponent();
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	void AddHealth(float Health);

	// TODO: Move this function into separate component (TeamComponent) or
	// into character class and access it through interface (ITeam)
	UFUNCTION(BLueprintCallable, BlueprintPure, Category = "HealthComponent")
	static bool IsFriends(const AActor* ActorA, const AActor* ActorB);

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

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
	bool IsDead() const;
};
