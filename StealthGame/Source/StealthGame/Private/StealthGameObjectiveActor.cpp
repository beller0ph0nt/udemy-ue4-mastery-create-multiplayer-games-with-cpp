// Fill out your copyright notice in the Description page of Project Settings.


#include "StealthGameObjectiveActor.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "StealthGameCharacter.h"

// Sets default values
AStealthGameObjectiveActor::AStealthGameObjectiveActor()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComponent;

	CollisionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphereComponent"));
	CollisionSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionSphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	CollisionSphereComponent->SetupAttachment(RootComponent);
}

void AStealthGameObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	PlayEffects();

	auto Character = Cast<AStealthGameCharacter>(OtherActor);
	if (Character)
	{
		Character->bIsCarryingObjective = true;

		Destroy();
	}
}

// Called when the game starts or when spawned
void AStealthGameObjectiveActor::BeginPlay()
{
	Super::BeginPlay();
}

void AStealthGameObjectiveActor::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupEffect, GetActorLocation());
}
