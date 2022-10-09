// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoopGameWeapon.h"
#include "CoopGameProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class COOPGAME_API ACoopGameProjectileWeapon : public ACoopGameWeapon
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AActor> ProjectileClass;

	virtual void Fire() override;
};
