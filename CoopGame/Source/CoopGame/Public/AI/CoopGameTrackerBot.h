#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "CoopGameTrackerBot.generated.h"

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

private:
	FVector GetNextPathPoint();
};
