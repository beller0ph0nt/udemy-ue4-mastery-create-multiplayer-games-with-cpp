#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "CoopGameGameModeBase.generated.h"


UCLASS()
class COOPGAME_API ACoopGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPostLoginSignature, APlayerController*, NewPlayer);
	FPostLoginSignature OnPostLogin;

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
