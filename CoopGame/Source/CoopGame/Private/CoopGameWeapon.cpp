// Fill out your copyright notice in the Description page of Project Settings.

#include "CoopGameWeapon.h"

#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

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
}

void ACoopGameWeapon::Fire()
{
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

		FHitResult HitResult;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, QueryParams))
		{
			AActor* HitActor = HitResult.GetActor();
			float Damage = 20.0;
			UGameplayStatics::ApplyPointDamage(HitActor, Damage, Start, HitResult, WeaponOwner->GetInstigatorController(), this, DamageType);

			if (ImpactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
			}

			TracerEndPoint = HitResult.ImpactPoint;
		}

		if (WeaponDebugDrawing)
		{
			DrawDebugLine(GetWorld(), Start, End, FColor::Cyan, false, 1.0f, 0.0f, 1.0f);
		}

		PlayFireEffects(TracerEndPoint);
	}
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
