// Copyright Epic Games, Inc. All Rights Reserved.

#include "MShooterCharacter.h"

#include <HeadMountedDisplayFunctionLibrary.h>
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/InputComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/Controller.h>
#include <GameFramework/SpringArmComponent.h>

#include "../Components/MShooterShootingComponent.h"

AMShooterCharacter::AMShooterCharacter()
{
	//Create Shooting Component
	ShootingComponent = CreateDefaultSubobject<UMShooterShootingComponent>(FName(TEXT("ShootingComponent")));
}

void AMShooterCharacter::Shoot()
{
	if (!ShootingComponent)
	{
		return;
	}

	ShootingComponent->TriggerShot();
}