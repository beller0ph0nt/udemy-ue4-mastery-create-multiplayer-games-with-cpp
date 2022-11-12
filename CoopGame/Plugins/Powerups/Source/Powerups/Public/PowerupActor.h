#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PowerupActor.generated.h"

UCLASS()
class POWERUPS_API APowerupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APowerupActor();

	void PowerupActivate(AActor* Actor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
	void OnActivate(AActor* PowerupActor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
	void OnPowerupTick();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
	void OnExpire(AActor* PowerupActor);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	float TimeBetweenPowerupTicks = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	int32 TotalNumberOfTicks = 0;

	UPROPERTY(ReplicatedUsing = OnRep_bActivated)
	bool bActivated = false;

	UFUNCTION()
	void OnRep_bActivated();

private:
	int32 NumberOfTicksProcessed = 0;
	FTimerHandle PowerupTickTimer;

	UPROPERTY(Replicated)
	AActor* ActorPickedupPowerup;

	void PowerupTickTimerHandler();
};
