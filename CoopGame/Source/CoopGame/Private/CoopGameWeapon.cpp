// Fill out your copyright notice in the Description page of Project Settings.

#include "CoopGameWeapon.h"

#include "Components/StaticMeshComponent.h"
#include "CoopGame.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "TimerManager.h"

static bool WeaponDebugDrawing = false;
static FAutoConsoleVariableRef CVAR_WeaponDebugDrawing(
	TEXT("COOP.WeaponDebugDrawing"),
	WeaponDebugDrawing,
	TEXT("Draw debug lines for weapons"),
	ECVF_Cheat);

ACoopGameWeapon::ACoopGameWeapon()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	MuzzleSocketName = "MuzzleSocket";
	TracerEndPointParameterName = "BeamEnd";

	bReplicates = true;
	NetUpdateFrequency = 66.6f;
	MinNetUpdateFrequency = 33.3f;
}

void ACoopGameWeapon::StartFire()
{
	bIsFiring = true;
	if (!GetWorldTimerManager().IsTimerActive(DelayBetweenShotsTimerHandle))
	{
		GetWorldTimerManager().SetTimer(DelayBetweenShotsTimerHandle, this, &ThisClass::FireHandler, DelayBetweenShots, true, 0.0f);
	}
}

void ACoopGameWeapon::EndFire()
{
	bIsFiring = false;
}

void ACoopGameWeapon::Fire()
{
	if (GetLocalRole() != ROLE_Authority)
	{
		ServerFire();
	}

	AActor* WeaponOwner = GetOwner();
	if (WeaponOwner)
	{
		FVector EyesLocation;
		FRotator EyesRotation;
		WeaponOwner->GetActorEyesViewPoint(EyesLocation, EyesRotation);

		FVector Start = EyesLocation;
		FVector End = Start + (EyesRotation.Vector() * 10000);
		FVector TracerEndPoint = End;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(WeaponOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		FHitResult HitResult;
		EPhysicalSurface PhysicalSurface = SurfaceType_Default;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, TRACE_CHANNEL_WEAPON, QueryParams))
		{
			AActor* HitActor = HitResult.GetActor();

			PhysicalSurface = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());

			float Damage = BaseDamage;
			if (PhysicalSurface == SURFACE_FLESHVULNERABLE)
			{
				Damage *= HeadShotDamageMultipier;
			}

			UGameplayStatics::ApplyPointDamage(HitActor, Damage, Start, HitResult, WeaponOwner->GetInstigatorController(), this, DamageType);

			PlayImpactEffects(PhysicalSurface, HitResult.ImpactPoint, HitResult.ImpactNormal);

			TracerEndPoint = HitResult.ImpactPoint;
		}

		if (WeaponDebugDrawing)
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Cyan, false, 1.0f, 0.0f, 1.0f);
		}

		PlayFireEffects(TracerEndPoint);

		if (GetLocalRole() == ROLE_Authority)
		{
			HitSync.ImpactPoint = TracerEndPoint;
			HitSync.ImpactNormal = HitResult.ImpactNormal;
			HitSync.PhysicalSurface = PhysicalSurface;
		}
	}
}

void ACoopGameWeapon::ServerFire_Implementation()
{
	Fire();
}

bool ACoopGameWeapon::ServerFire_Validate()
{
	return true;
}

void ACoopGameWeapon::OnRep_HitSync()
{
	PlayFireEffects(HitSync.ImpactPoint);
	PlayImpactEffects(HitSync.PhysicalSurface, HitSync.ImpactPoint, HitSync.ImpactNormal);
}

void ACoopGameWeapon::PlayFireEffects(const FVector& TracerEndPoint)
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, SkeletalMeshComponent, MuzzleSocketName);
	}

	if (TracerEffect)
	{
		FVector MuzzleLocation = SkeletalMeshComponent->GetSocketLocation(MuzzleSocketName);
		UParticleSystemComponent* TracerComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
		if (TracerComponent)
		{
			TracerComponent->SetVectorParameter(TracerEndPointParameterName, TracerEndPoint);
		}
	}

	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		APlayerController* PlayerController = MyOwner->GetInstigatorController<APlayerController>();
		if (PlayerController)
		{
			PlayerController->ClientStartCameraShake(FireCameraShake);
		}
	}
}

void ACoopGameWeapon::PlayImpactEffects(EPhysicalSurface PhysicalSurface, const FVector& ImpactPoint, const FVector& ImpactNormal)
{
	UParticleSystem* SelectedEffect = nullptr;
	switch (PhysicalSurface)
	{
	case SURFACE_FLESHDEFAULT:
	case SURFACE_FLESHVULNERABLE:
		SelectedEffect = FleshImpactEffect;
		break;
	default:
		SelectedEffect = DefaultImpactEffect;
		break;
	}

	if (SelectedEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, ImpactPoint, ImpactNormal.Rotation());
	}
}

void ACoopGameWeapon::FireHandler()
{
	if (bIsFiring)
	{
		Fire();
	}
	else
	{
		GetWorldTimerManager().ClearTimer(DelayBetweenShotsTimerHandle);
	}
}

void ACoopGameWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ACoopGameWeapon, HitSync, COND_SkipOwner);
}
