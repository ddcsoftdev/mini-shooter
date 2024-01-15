// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MShooterController.generated.h"



DECLARE_MULTICAST_DELEGATE_OneParam(FSendShootRequest, bool);
DECLARE_MULTICAST_DELEGATE(FSendAimRequest);

UCLASS()
class MINISHOOTER_API AMShooterController : public APlayerController
{
	GENERATED_BODY()
public:
	AMShooterController();

	/**
	* Delegate for shooting
	*/
	FSendShootRequest ShootDelegate;

	/**
	* Delegate for aiming
	*/
	FSendAimRequest AimDelegate;

	/**
	* Override with Super so we can add more inputs
	*/
	virtual void SetupInputComponent() override;

protected:

	/**
	* Broadcast Shoot Delegate to Start or Stop
	*/
	UFUNCTION()
	void RequestShoot(bool bStart);


	/**
	* Broadcast Aim Delegate to Toggle Start or Stop
	*/
	UFUNCTION()
	void RequestAim();

	virtual void BeginPlay() override;

};
