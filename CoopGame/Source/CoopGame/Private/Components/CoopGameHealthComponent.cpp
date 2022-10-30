#include "Components/CoopGameHealthComponent.h"

#include "Net/UnrealNetwork.h"

UCoopGameHealthComponent::UCoopGameHealthComponent()
{
	SetIsReplicatedByDefault(true);
}

void UCoopGameHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = DefaultHealth;

	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor* Owner = GetOwner();
		if (Owner)
		{
			Owner->OnTakeAnyDamage.AddDynamic(this, &ThisClass::TakeAnyDamageHandler);
		}
	}
}

void UCoopGameHealthComponent::TakeAnyDamageHandler(
	AActor* DamagedActor,
	float Damage,
	const UDamageType* DamageType,
	AController* InstigatedBy,
	AActor* DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);
	UE_LOG(LogTemp, Log, TEXT("UCoopGameHealthComponent::TakeAnyDamageHandler Health: %f"), Health);
	OnHealthChanged.Broadcast(this, Health, Damage, DamageType, InstigatedBy, DamageCauser);
}

void UCoopGameHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCoopGameHealthComponent, Health);
}
