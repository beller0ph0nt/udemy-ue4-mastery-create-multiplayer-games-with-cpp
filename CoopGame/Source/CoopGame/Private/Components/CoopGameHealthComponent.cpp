#include "Components/CoopGameHealthComponent.h"

#include "CoopGameGameModeBase.h"
#include "Net/UnrealNetwork.h"

UCoopGameHealthComponent::UCoopGameHealthComponent()
{
	SetIsReplicatedByDefault(true);
}

float UCoopGameHealthComponent::GetHealth() const
{
	return HealthComponentSync.Health;
}

void UCoopGameHealthComponent::AddHealth(float Health)
{
	if (Health <= 0.0f || HealthComponentSync.Health <= 0.0f)
	{
		return;
	}

	HealthComponentSync.Health = FMath::Clamp(HealthComponentSync.Health + Health, 0.0f, DefaultHealth);
	HealthComponentSync.Damage = -Health;

	OnRep_HealthComponentSync();
}

void UCoopGameHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	HealthComponentSync.Health = DefaultHealth;
	HealthComponentSync.Damage = 0;

	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor* Owner = GetOwner();
		if (Owner)
		{
			Owner->OnTakeAnyDamage.AddDynamic(this, &ThisClass::TakeAnyDamageHandler);
		}
	}
}

void UCoopGameHealthComponent::OnRep_HealthComponentSync()
{
	UE_LOG(LogTemp, Log, TEXT("UCoopGameHealthComponent::TakeAnyDamageHandler Health: %f"), HealthComponentSync.Health);
	OnHealthChanged.Broadcast(this, HealthComponentSync.Health, HealthComponentSync.Damage);
}

void UCoopGameHealthComponent::TakeAnyDamageHandler(
	AActor* DamagedActor,
	float Damage,
	const UDamageType* DamageType,
	AController* InstigatedBy,
	AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead())
	{
		return;
	}

	HealthComponentSync.Health = FMath::Clamp(HealthComponentSync.Health - Damage, 0.0f, DefaultHealth);
	HealthComponentSync.Damage = Damage;

	OnRep_HealthComponentSync();

	if (IsDead())
	{
		ACoopGameGameModeBase* GM = GetWorld()->GetAuthGameMode<ACoopGameGameModeBase>();
		if (GM && InstigatedBy)
		{
			GM->OnPlayerKill.Broadcast(DamagedActor, InstigatedBy->GetPawn());
		}
	}
}

bool UCoopGameHealthComponent::IsDead() const
{
	return HealthComponentSync.Health <= 0.0f;
}

void UCoopGameHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCoopGameHealthComponent, HealthComponentSync);
}
