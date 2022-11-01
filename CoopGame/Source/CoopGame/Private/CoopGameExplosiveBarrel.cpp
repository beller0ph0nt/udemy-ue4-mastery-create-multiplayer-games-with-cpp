#include "CoopGameExplosiveBarrel.h"

#include "Components/CoopGameHealthComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystem.h"
#include "PhysicsEngine/RadialForceComponent.h"

ACoopGameExplosiveBarrel::ACoopGameExplosiveBarrel()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	MeshComponent->SetSimulatePhysics(true);
	MeshComponent->SetMassOverrideInKg(NAME_None, 100.0f);
	MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	RootComponent = MeshComponent;

	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));
	RadialForceComponent->bIgnoreOwningActor = true;
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->Radius = 250.0f;
	RadialForceComponent->ImpulseStrength = 1000.0f;
	RadialForceComponent->SetupAttachment(RootComponent);

	HealthComponent = CreateDefaultSubobject<UCoopGameHealthComponent>(TEXT("HealthComponent"));

	SetReplicates(true);
	SetReplicateMovement(true);
}

void ACoopGameExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		HealthComponent->OnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChanged);
	}
}

void ACoopGameExplosiveBarrel::OnHealthChanged(UCoopGameHealthComponent* Component,	float Health, float Damage)
{
	if (Health <= 0.0f && !bIsExploded)
	{
		bIsExploded = true;

		ApplyExplodePhysics();
	}
}

void ACoopGameExplosiveBarrel::OnRep_bIsExploded()
{
	PlayExplodeEffects();
}

void ACoopGameExplosiveBarrel::PlayExplodeEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplodeEffect, GetActorLocation(), GetActorRotation());
	MeshComponent->SetMaterial(0, ExplodedMaterial);
}

void ACoopGameExplosiveBarrel::ApplyExplodePhysics()
{
	MeshComponent->AddImpulse(FVector::UpVector * ExplodeImpulse);
	RadialForceComponent->FireImpulse();
}

void ACoopGameExplosiveBarrel::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACoopGameExplosiveBarrel, bIsExploded);
}
