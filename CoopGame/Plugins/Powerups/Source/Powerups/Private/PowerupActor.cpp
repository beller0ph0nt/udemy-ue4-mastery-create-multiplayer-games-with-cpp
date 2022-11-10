#include "PowerupActor.h"

APowerupActor::APowerupActor()
{
}

void APowerupActor::BeginPlay()
{
	Super::BeginPlay();
}

void APowerupActor::PowerupTickTimerHandler()
{
	NumberOfTicksProcessed++;
	if (TotalNumberOfTicks <= NumberOfTicksProcessed)
	{
		GetWorldTimerManager().ClearTimer(PowerupTickTimer);
		OnExpire();
	}
	else
	{
		OnPowerupTick();
	}
}

void APowerupActor::PowerupActivate()
{
	if (0.0f < TimeBetweenPowerupTicks)
	{
		OnActivate();
		GetWorldTimerManager().SetTimer(PowerupTickTimer, this, &ThisClass::PowerupTickTimerHandler, TimeBetweenPowerupTicks, true, 0.0f);
	}
}
