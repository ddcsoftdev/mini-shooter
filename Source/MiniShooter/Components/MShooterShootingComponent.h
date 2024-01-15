// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MShooterShootingComponent.generated.h"

class AMShooterProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINISHOOTER_API UMShooterShootingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMShooterShootingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/**
	* Timer Handle for Shooting timer
	*/
	UPROPERTY()
		FTimerHandle ShootingTimer;

	/**
	* Override the speed of the projectile
	*/
	UPROPERTY(EditAnywhere, Category = "Override Projectile Speed")
	bool bOverrideProjectileSpeed{ true };

	/**
	* Speed for Overriden projectile Speed
	*/
	UPROPERTY(EditAnywhere, Category = "Override Projectile Speed")
	float ProjectileSpeed{ 20.f };

	/**
	* Shooting cadence
	*/
	UPROPERTY(EditAnywhere, Category = "Override Projectile Speed")
	float ShootingCadence{ 1.f };

	/**
	* Reference for Projectile BP
	*/
	UPROPERTY()
	TSubclassOf<AMShooterProjectile> ProjectileBPReference;

	/**
	* Trigger Shot
	*/
	UFUNCTION()
		void TriggerShot();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	* Start Shoot Loop
	*/
	UFUNCTION()
		void StartShooting();

	/**
	* Start Shoot Loop
	*/
	UFUNCTION()
		void StopShooting();
		
};
