// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "../MiniShooterCharacter.h"
#include "MShooterCharacter.generated.h"


class UMShooterShootingComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FRequestNearestEnemy, class AActor*);
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
	*/
	UFUNCTION()
	AActor* RequestGetAimedEnemy();

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
	UPROPERTY(EditAnywhere)
		float TimeToAutoStopAiming{ 0.25f };

	/**
	* Shooting method that trigger the Shooting Component
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
	*/
	UFUNCTION()
	void GetNearestEnemy(AActor* NearestEnemy);

	virtual void BeginPlay() override;
};

