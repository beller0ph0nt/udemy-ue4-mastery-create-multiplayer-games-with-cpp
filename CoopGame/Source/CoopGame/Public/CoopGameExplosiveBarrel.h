#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CoopGameExplosiveBarrel.generated.h"

class UCoopGameHealthComponent;
class URadialForceComponent;

UCLASS()
class COOPGAME_API ACoopGameExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	ACoopGameExplosiveBarrel();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthChanged(UCoopGameHealthComponent* Component, float Health, float Damage);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCoopGameHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	URadialForceComponent* RadialForceComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* ExplodeEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UMaterialInterface* ExplodedMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Explode")
	float ExplodeImpulse = 100000.0f;

	UPROPERTY(ReplicatedUsing = OnRep_bIsExploded)
	bool bExploded = false;

	UFUNCTION()
	void OnRep_bIsExploded();
private:

	UFUNCTION()
	void PlayExplodeEffects();

	UFUNCTION()
	void ApplyExplodePhysics();
};
