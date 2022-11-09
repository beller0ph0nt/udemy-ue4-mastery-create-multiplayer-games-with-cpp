#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PickupActor.generated.h"

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

	UPROPERTY(EditDefaultsOnly, Category = "Pickup|Sizes")
	float DecalSize = 75.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup|Sizes")
	float DecalThickness = 64.0f;

	virtual void BeginPlay() override;
};
