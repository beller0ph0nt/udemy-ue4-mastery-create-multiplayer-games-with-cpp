// Fill out your copyright notice in the Description page of Project Settings.


#include "StealthGameExtractionZone.h"

#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"

AStealthGameExtractionZone::AStealthGameExtractionZone()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	BoxComponent->SetBoxExtent(FVector(200.0));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::BoxComponentBeginOverlap);
	BoxComponent->SetupAttachment(RootComponent);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->DecalSize = FVector(200.0);
	DecalComponent->SetupAttachment(BoxComponent);
}

void AStealthGameExtractionZone::BoxComponentBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Extraction zone have been overlapped"));
}
