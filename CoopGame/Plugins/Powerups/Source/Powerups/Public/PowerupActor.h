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

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
	void OnActivate();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
	void OnPowerupTick();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerup")
	void OnExpire();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	float TimeBetweenPowerupTicks = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	int32 TotalNumberOfTicks = 0;

	virtual void BeginPlay() override;

private:
	int32 NumberOfTicksProcessed = 0;
	FTimerHandle PowerupTickTimer;

	void PowerupTickTimerHandler();
	void PowerupActivate();
};
