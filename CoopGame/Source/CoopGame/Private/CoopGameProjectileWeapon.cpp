// Fill out your copyright notice in the Description page of Project Settings.


#include "CoopGameProjectileWeapon.h"

void ACoopGameProjectileWeapon::Fire()
{
	AActor* WeaponOwner = GetOwner();
	if (WeaponOwner && ProjectileClass)
	{
		FVector EyesLocation;
		FRotator EyesRotation;
		WeaponOwner->GetActorEyesViewPoint(EyesLocation, EyesRotation);

		FVector MuzzleLocation = SkeletalMeshComponent->GetSocketLocation(MuzzleSocketName);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, EyesRotation);
	}
}
