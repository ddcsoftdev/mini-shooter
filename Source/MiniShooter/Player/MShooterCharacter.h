// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "../MiniShooterCharacter.h"

#include "MShooterCharacter.generated.h"


class UMShooterShootingComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FRequestNearestEnemy, class AActor*);

/**
* Character class derived from class created by UE for project
* Core for Player's mechanics and functionality
*/
UCLASS(config=Game)
class AMShooterCharacter : public AMiniShooterCharacter
{
	GENERATED_BODY()

public:

	AMShooterCharacter();

	/**
	* Delegate to Request Nearest Enemy
	*/
	FRequestNearestEnemy RequestNearestEnemyDelegate;

	/**
	* Get Current Aimed Enemy
	* This method does not Sanitize so nullptr is plausible
	*/
	UFUNCTION()
		AActor* RequestGetAimedEnemy();

	/**
	* Set new Shooting speed
 	*
  	* @param NewSpeed Value that sets the value for ShootingSpeed
	*/
	UFUNCTION()
		void SetShootingSpeed(float NewSpeed);

	/**
	* Get new Shooting speed
	* It returns -1.f if error occurs
	*/
	UFUNCTION()
		float GetShootingSpeed();

protected:

	/**
	* Shooting Component
	*/
	UPROPERTY(EditAnywhere)
		UMShooterShootingComponent* ShootingComponent;

	/**
	* Stop Aiming TimeHandle so you can aim before shooting
	*/
	UPROPERTY(EditAnywhere)
		FTimerHandle StopAimingTimeHandle;

	/**
	* Time for Aiming to reset if not Shooting
	*/
	UPROPERTY(EditAnywhere, Category = "Character Config")
		float TimeToAutoStopAiming{ 0.25f };

	/**
	* Shooting method that trigger the Shooting Component
 	*
  	* @param bStart Value to start shooting
	*/
	UFUNCTION()
		void Shoot(bool bStart);

	/**
	* Aiming method that start aiming at Enemies
	*/
	UFUNCTION()
		void Aim();

	/**
	* Recieve nearest Enemy
 	*
  	* @param NearestEnemy Nearest Enemy to aim at
	*/
	UFUNCTION()
		void GetNearestEnemy(AActor* NearestEnemy);

	virtual void BeginPlay() override;
};

