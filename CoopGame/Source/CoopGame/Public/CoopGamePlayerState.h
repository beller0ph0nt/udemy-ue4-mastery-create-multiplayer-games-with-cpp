#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "CoopGamePlayerState.generated.h"

UCLASS()
class COOPGAME_API ACoopGamePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	void AddScore(float ScoreDelta);

protected:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "PlayerState")
	float PlayerScore = 0.0f;
};
