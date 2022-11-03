#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "CoopGameTrackerBot.generated.h"

class UCoopGameHealthComponent;
class UMaterialInstanceDynamic;
class UParticleSystem;
class UStaticMeshComponent;

UCLASS()
class COOPGAME_API ACoopGameTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	ACoopGameTrackerBot();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UCoopGameHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float MovementForce = 1000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	bool bAccelerationChange = true;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float RequiredDistanceToTarget = 100.0f;

	UPROPERTY()
	UMaterialInstanceDynamic* MaterialInstanceDynamic;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplosionDamage = 40.0f;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplosionRadius = 200.0f;

private:
	FVector GetNextPathPoint();
	void SelfExplode();

	UFUNCTION()
	void OnHealthChanged(UCoopGameHealthComponent* OwnerHealthComponent, float Health, float Damage);

	FVector NextPathPoint;
	double DisableNavigationDistance = 10.0;
	bool bIsExploded = false;
};
