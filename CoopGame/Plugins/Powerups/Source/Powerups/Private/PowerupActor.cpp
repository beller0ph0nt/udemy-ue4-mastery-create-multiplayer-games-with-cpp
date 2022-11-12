#include "PowerupActor.h"

#include "Net/UnrealNetwork.h"

APowerupActor::APowerupActor()
{
	SetReplicates(true);
}

void APowerupActor::PowerupActivate()
{
	if (0.0f < TimeBetweenPowerupTicks)
	{
		bActivated = true;
		OnRep_bActivated();

		GetWorldTimerManager().SetTimer(PowerupTickTimer, this, &ThisClass::PowerupTickTimerHandler, TimeBetweenPowerupTicks, true, 0.0f);
	}
}

void APowerupActor::OnRep_bActivated()
{
	if (!IsNetMode(ENetMode::NM_DedicatedServer))
	{
		if (bActivated)
		{
			OnActivate();
		}
	}
}

void APowerupActor::PowerupTickTimerHandler()
{
	if (TotalNumberOfTicks <= NumberOfTicksProcessed)
	{
		GetWorldTimerManager().ClearTimer(PowerupTickTimer);
		OnExpire();

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
}

