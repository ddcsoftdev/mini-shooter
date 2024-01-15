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
#include "MShooterController.h"

AMShooterCharacter::AMShooterCharacter()
{
	//Create Shooting Component
	ShootingComponent = CreateDefaultSubobject<UMShooterShootingComponent>(FName(TEXT("ShootingComponent")));
}

void AMShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Bind Shoot Input
	if (AMShooterController* PlayerController = Cast<AMShooterController>(GetController()))
	{
		PlayerController->ShootDelegate.AddUObject(this, &AMShooterCharacter::Shoot);
	}
}

void AMShooterCharacter::Shoot(bool bStart)
{
	if (!ShootingComponent)
	{
		return;
	}
	if (bStart)
	{
		ShootingComponent->StartShooting();
	}
	else
	{
		ShootingComponent->StopShooting();
	}
	
}