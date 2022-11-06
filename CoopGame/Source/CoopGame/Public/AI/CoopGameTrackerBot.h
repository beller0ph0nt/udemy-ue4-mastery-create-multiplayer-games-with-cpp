#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "CoopGameTrackerBot.generated.h"

class UCoopGameHealthComponent;
class UMaterialInstanceDynamic;
class UParticleSystem;
class USphereComponent;
class USoundCue;
class UStaticMeshComponent;

UCLASS()
class COOPGAME_API ACoopGameTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	ACoopGameTrackerBot();

	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UCoopGameHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float MovementForce = 1000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	bool bAccelerationChange = true;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float RequiredDistanceToTarget = 100.0f;

	UPROPERTY()
	UMaterialInstanceDynamic* MaterialInstanceDynamic;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplosionDamage = 40.0f;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float ExplosionRadius = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot|Sounds")
	USoundCue* StartSelfDestructSound;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot|Sounds")
	USoundCue* ExplosionSound;

private:
	FVector GetNextPathPoint();
	void SelfExplode();
	void UpdateDamagePowerLevelBoost();

	UFUNCTION()
	void OnHealthChanged(UCoopGameHealthComponent* OwnerHealthComponent, float Health, float Damage);

	UFUNCTION()
	void HandleNewPlayer(APlayerController* NewPlayer);

	UFUNCTION()
	void OnRep_bSelfDestructionStarted();

	UFUNCTION()
	void OnRep_bIsExploded();
	
	const float ClientsSynchronizationTimeSpanBeforeBotDestroy = 1.0f;
	const float DamageBoostPerBot = 10.0f;
	const float MaxDamageBoost = 40.0f;

	FVector NextPathPoint;
	double DisableNavigationDistance = 10.0;
	FTimerHandle SelfDestructTimerHandler;
	float SelfDestructTimerRate = 0.5f;
	bool bAtLeastOnePlayerInTheGame = false;
	bool bSelfDestructionStarted = false;
	int BotsCounter = 0;
	float DamageBoost = 0.0f;

	UPROPERTY(ReplicatedUsing = OnRep_bIsExploded)
	bool bIsExploded = false;
};
