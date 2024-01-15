// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MShooterController.generated.h"



DECLARE_MULTICAST_DELEGATE(FSendShootRequest);

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
	* Override with Super so we can add more inputs
	*/
	virtual void SetupInputComponent() override;

protected:

	/**
	* Broadcast Shoot Delegate
	*/
	UFUNCTION()
	void RequestShoot();

	virtual void BeginPlay() override;

};
