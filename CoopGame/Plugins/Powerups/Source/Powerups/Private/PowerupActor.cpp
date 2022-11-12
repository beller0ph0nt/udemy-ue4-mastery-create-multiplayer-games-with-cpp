#include "PowerupActor.h"

APowerupActor::APowerupActor()
{
}

void APowerupActor::PowerupActivate()
{
	if (0.0f < TimeBetweenPowerupTicks)
	{
		OnActivate();
		GetWorldTimerManager().SetTimer(PowerupTickTimer, this, &ThisClass::PowerupTickTimerHandler, TimeBetweenPowerupTicks, true, 0.0f);
	}
}

void APowerupActor::BeginPlay()
{
	Super::BeginPlay();
}

void APowerupActor::PowerupTickTimerHandler()
{
	if (TotalNumberOfTicks <= NumberOfTicksProcessed)
	{
		GetWorldTimerManager().ClearTimer(PowerupTickTimer);
		OnExpire();
	}
	else
	{
		OnPowerupTick();
	}

	NumberOfTicksProcessed++;
}
