#include "PowerupActor.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"

APowerupActor::APowerupActor()
{
	//SetReplicates(true);
	bReplicates = true;
}

void APowerupActor::PowerupActivate(AActor* Actor)
{
	ActorPickedupPowerup = Actor;
	if (0.0f < TimeBetweenPowerupTicks)
	{
		bActivated = true;
		OnRep_bActivated();

		GetWorldTimerManager().SetTimer(PowerupTickTimer, this, &ThisClass::PowerupTickTimerHandler, TimeBetweenPowerupTicks, true, 0.0f);
	}
}

void APowerupActor::OnRep_bActivated()
{
	//if (!IsNetMode(ENetMode::NM_DedicatedServer))
	{
		if (bActivated)
		{
			UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("OnRep_bActivated!!!")));
			OnActivate(ActorPickedupPowerup);
		}
	}
}

void APowerupActor::PowerupTickTimerHandler()
{
	if (TotalNumberOfTicks <= NumberOfTicksProcessed)
	{
		GetWorldTimerManager().ClearTimer(PowerupTickTimer);
		OnExpire(ActorPickedupPowerup);

		bActivated = true;
		OnRep_bActivated();
	}
	else
	{
		OnPowerupTick();
	}

	NumberOfTicksProcessed++;
}

void APowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APowerupActor, bActivated);
	DOREPLIFETIME(APowerupActor, ActorPickedupPowerup);
}

