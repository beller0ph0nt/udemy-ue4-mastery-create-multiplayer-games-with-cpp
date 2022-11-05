#include "CoopGameGameModeBase.h"

void ACoopGameGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	OnPostLogin.Broadcast(NewPlayer);
}
