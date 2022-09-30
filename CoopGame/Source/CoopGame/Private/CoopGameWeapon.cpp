// Fill out your copyright notice in the Description page of Project Settings.

#include "CoopGameWeapon.h"

#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

ACoopGameWeapon::ACoopGameWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
}

void ACoopGameWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACoopGameWeapon::BeginPlay()
{
	Super::BeginPlay();
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
		}

		DrawDebugLine(GetWorld(), Start, End, FColor::Cyan, false, 1.0f, 0.0f, 1.0f);
	}
}
