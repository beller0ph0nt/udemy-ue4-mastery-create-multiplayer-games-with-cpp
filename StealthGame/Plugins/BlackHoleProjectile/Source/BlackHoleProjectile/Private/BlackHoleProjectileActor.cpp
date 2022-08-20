// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHoleProjectileActor.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

ABlackHoleProjectileActor::ABlackHoleProjectileActor()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(RootComponent);

	AnnihilationSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AnnihilationSphere"));
	AnnihilationSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AnnihilationSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	AnnihilationSphere->OnComponentBeginOverlap.AddDynamic(this, &ABlackHoleProjectileActor::OnAnnihilationSphereOverlap);
	AnnihilationSphere->SetupAttachment(MeshComponent);

	GravitySphere = CreateDefaultSubobject<USphereComponent>(TEXT("GravitySphere"));
	GravitySphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GravitySphere->SetupAttachment(MeshComponent);
}

void ABlackHoleProjectileActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ThisClass::OnExplosionBegin, TimeUntilExplosionBegin.GetTotalSeconds());
}

void ABlackHoleProjectileActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsExplosionBegin)
	{
		TArray<UPrimitiveComponent*> OverlappedComponents;
		GravitySphere->GetOverlappingComponents(OverlappedComponents);
		for (const auto Component : OverlappedComponents)
		{
			if (Component && Component->IsSimulatingPhysics())
			{
				Component->AddRadialForce(
					GetActorLocation(),
					GravitySphere->GetScaledSphereRadius(),
					GravityForceStrength,
					ERadialImpulseFalloff::RIF_Linear);
			}
		}
	}
}

void ABlackHoleProjectileActor::OnExplosionBegin()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandler);
	bIsExplosionBegin = true;
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ThisClass::OnExplosionEnd, ExplosionDuration.GetTotalSeconds());
}

void ABlackHoleProjectileActor::OnExplosionEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandler);
	bIsExplosionBegin = false;
	Destroy();
}

void ABlackHoleProjectileActor::OnAnnihilationSphereOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}
