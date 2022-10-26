#include "CoopGameExplosiveBarrel.h"

#include "Components/CoopGameHealthComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
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
}

void ACoopGameExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChanged);
}

void ACoopGameExplosiveBarrel::OnHealthChanged(
	UCoopGameHealthComponent* Component,
	float Health,
	float Damage,
	const UDamageType* DamageType,
	AController* InstigatedBy,
	AActor* DamageCauser)
{
	if (Health <= 0.0f && !bIsExploaded)
	{
		bIsExploaded = true;

		UGameplayStatics::SpawnEmitterAtLocation(this, ExplodeEffect, GetActorLocation(), GetActorRotation());
		MeshComponent->AddImpulse(FVector::UpVector * ExplodeImpulse);
		MeshComponent->SetMaterial(0, ExplodedMaterial);
		RadialForceComponent->FireImpulse();
	}
}
