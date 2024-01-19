// Copyright Epic Games, Inc. All Rights Reserved.

#include "MShooterCharacter.h"

#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/InputComponent.h>
#include <Kismet/Gameplaystatics.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/Controller.h>
#include <GameFramework/SpringArmComponent.h>

#include "../Components/MShooterShootingComponent.h"
#include "../MiniShooterGameMode.h"
#include "MShooterController.h"

AMShooterCharacter::AMShooterCharacter()
{
	//Create Shooting Component
	ShootingComponent = CreateDefaultSubobject<UMShooterShootingComponent>(FName(TEXT("ShootingComponent")));
}

void AMShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Bind Shoot and Aim Delegate
	if (AMShooterController* PlayerController = Cast<AMShooterController>(GetController()))
	{
		PlayerController->ShootDelegate.AddUObject(this, &AMShooterCharacter::Shoot);
		PlayerController->AimDelegate.AddUObject(this, &AMShooterCharacter::Aim);
	}
	//Bind Send Enemy Delegate to Recieve new Enemy upon Aim State start
	if (AMiniShooterGameMode* GameMode = Cast<AMiniShooterGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->SendEnemyDelegate.AddUObject(this, &AMShooterCharacter::GetNearestEnemy);
	}
}

void AMShooterCharacter::Shoot(bool bStart)
{
	if (ensureMsgf(IsValid(ShootingComponent) && IsValid(GetWorld()), TEXT("%s faced error at Runtime"), *GetClass()->GetName()))
	{
		//If Delegate ends true, then start shooting
		if (bStart)
		{
			ShootingComponent->StartShooting();
			GetWorld()->GetTimerManager().ClearTimer(StopAimingTimeHandle);
		}
		else
		{
			ShootingComponent->StopShooting();
			//Allow Aim to perdure some time without shooting so it's not too sudden
			GetWorld()->GetTimerManager().SetTimer(StopAimingTimeHandle, [&]() {ShootingComponent->StopAiming(); }, TimeToAutoStopAiming, false);
		}
	}
}

void AMShooterCharacter::Aim()
{
	if (ensureMsgf(IsValid(ShootingComponent), TEXT("%s faced error at Runtime"), *GetClass()->GetName()))
	{
		//Request Nearest Enemy to GameMode to start Aiming
		RequestNearestEnemyDelegate.Broadcast(this);
	}
}

void AMShooterCharacter::GetNearestEnemy(AActor* NearestEnemy)
{
	if (ensureMsgf(IsValid(ShootingComponent) && IsValid(GetWorld()), TEXT("%s faced error at Runtime"), *GetClass()->GetName()))
	{
		//Check if there is any Enemy left to aim at
		if (IsValid(NearestEnemy))
		{
			ShootingComponent->StartAiming(NearestEnemy);
			//Start a TimerHandle to cancel Aiming state in case player stops shooting
			if (!ShootingComponent->GetIsShooting())
			{
				GetWorld()->GetTimerManager().SetTimer(StopAimingTimeHandle, [&]() {ShootingComponent->StopAiming(); }, TimeToAutoStopAiming, false);
			}
		}
		else
		{
			//If there is no Enemy left to aim, then stop Aiming state
			ShootingComponent->StopAiming();
		}
	}
}

AActor* AMShooterCharacter::RequestGetAimedEnemy()
{
	//Get the Enemy that the Player is currently aiming at
	if (ensureMsgf(IsValid(ShootingComponent), TEXT("%s faced error at Runtime"), *GetClass()->GetName()))
	{
		return ShootingComponent->GetCurrentAimedEnemy();
	}
	return nullptr;	
}

void AMShooterCharacter::SetShootingSpeed(float NewSpeed)
{
	if (ensureMsgf(IsValid(ShootingComponent), TEXT("%s faced error at Runtime"), *GetClass()->GetName()))
	{
		//Sets the new Shooting Speed for the Player within the Shooting Component
		ShootingComponent->SetShootingSpeed(NewSpeed);
		//Resets the Shooting Cycle within Component in order to apply new Shooting Speeed value
		ShootingComponent->RestartShootingTimer();
	}
}

float AMShooterCharacter::GetShootingSpeed()
{
	if (ensureMsgf(IsValid(ShootingComponent), TEXT("%s faced error at Runtime"), *GetClass()->GetName()))
	{
		//Gets current Shooting Speed from Shooting Component
		return ShootingComponent->GetShootingSpeed();
	}
	return -1.f;
}
