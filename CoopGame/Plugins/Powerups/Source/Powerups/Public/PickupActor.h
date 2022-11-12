#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PickupActor.generated.h"

class APowerupActor;
class UDecalComponent;
class USphereComponent;

UCLASS()
class POWERUPS_API APickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APickupActor();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDecalComponent* DecalComponent;

	UPROPERTY(EditInstanceOnly, Category = "Pickup|Powerup")
	TSubclassOf<APowerupActor> PowerupClass;

	UPROPERTY(EditInstanceOnly, Category = "Pickup|Powerup")
	float RespawnPowerupInterval = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup|Sizes")
	float DecalSize = 75.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup|Sizes")
	float DecalThickness = 64.0f;

	virtual void BeginPlay() override;

private:
	APowerupActor* PowerupInstance;
	FTimerHandle RespawnPowerupTimer;

	UFUNCTION()
	void RespawnPowerup();
};
