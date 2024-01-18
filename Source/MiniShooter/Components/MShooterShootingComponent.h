// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>

#include "MShooterShootingComponent.generated.h"

class AMShooterProjectile;

/**
* Component used to give an Actor Shooting functionality
* It handles Shooting and Aiming
* It adds some more functionality if it detects the Player holding the Component
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINISHOOTER_API UMShooterShootingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMShooterShootingComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	* Start Shoot Loop
	*/
	UFUNCTION()
		void StartShooting();

	/**
	* Stop Shoot Loop
	*/
	UFUNCTION()
		void StopShooting();

	/**
	* Start Aiming
 	*
  	* @param NearestEnemy Sends the target to aim to
	*/
	UFUNCTION()
		void StartAiming(AActor* NearestEnemy);

	/**
	* Stop Aiming
	*/
	UFUNCTION()
		void StopAiming();

	/**
	* Get Is Shooting
 	*
  	* @return If Actor is shooting
	*/
	UFUNCTION()
		bool GetIsShooting() { return bIsShooting; };

	/**
	* Get Is Aiming
 	*
  	* @return If Actor is aiming
	*/
	UFUNCTION()
		bool GetIsAiming() { return bIsAiming; };

	/**
	* Get Current Aimed Enemy
 	*
  	* @return The Enemy that is being currently aimed at
	*/
	UFUNCTION()
		AActor* GetCurrentAimedEnemy() { return CurrentAimedEnemy; };

	/**
	* Set Projectile Speed
 	*
  	* @param NewSpeed Value for the new ShootingSpeed
	*/
	UFUNCTION()
		void SetShootingSpeed(float NewSpeed) { ShootingSpeed = NewSpeed; };

	/**
	* Get Projectile Speed
 	*
  	* @return The current value for ShootingSpeed
	*/
	UFUNCTION()
		float GetShootingSpeed() { return ShootingSpeed; };

	/**
	* Restart Shoot cycle for actions like change Shooting Speed
	*/
	UFUNCTION()
		void RestartShootingTimer();

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
	UPROPERTY(EditAnywhere, Category = "Shooting Config")
		bool bOverrideProjectileSpeed{ true };

	/**
	* Speed for overriding the Projectile Speed
	*/
	UPROPERTY(EditAnywhere, Category = "Shooting Config")
		float ProjectileSpeed{ 40.f };

	/**
	* Shooting cadence
	*/
	UPROPERTY(EditAnywhere, Category = "Shooting Config")
		float ShootingSpeed{ 5.f };

	/**
	* Is Player bShooting
	*/
	UPROPERTY()
		bool bIsShooting{ false };

	/**
	* Is Player bAiming
	*/
	UPROPERTY()
		bool bIsAiming{ false };

	/**
	* Current Aimed Enemy to Player
	*/
	UPROPERTY()
		AActor* CurrentAimedEnemy;

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

	/**
	* Aiming Frame rotation
	*/
	UFUNCTION()
		void AimingRotationAdjustment();
};
