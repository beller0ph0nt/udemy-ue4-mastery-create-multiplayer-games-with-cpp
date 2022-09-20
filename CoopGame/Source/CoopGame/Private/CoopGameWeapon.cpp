// Fill out your copyright notice in the Description page of Project Settings.

#include "CoopGameWeapon.h"

#include "Components/StaticMeshComponent.h"

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
