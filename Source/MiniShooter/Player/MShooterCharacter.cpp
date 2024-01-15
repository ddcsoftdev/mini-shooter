// Copyright Epic Games, Inc. All Rights Reserved.

#include "MShooterCharacter.h"

#include <HeadMountedDisplayFunctionLibrary.h>
#include <Camera/CameraComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/InputComponent.h>
#include <Kismet/Gameplaystatics.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/Controller.h>
#include <GameFramework/SpringArmComponent.h>

#include "../Components/MShooterShootingComponent.h"
#include "MShooterController.h"
#include "../MiniShooterGameMode.h"

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
		PlayerController->AimDelegate.AddUObject(this, &AMShooterCharacter::Aim);
	}

	if (AMiniShooterGameMode* GameMode = Cast<AMiniShooterGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->SendEnemyDelegate.AddUObject(this, &AMShooterCharacter::GetNearestEnemy);
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

		GetWorld()->GetTimerManager().ClearTimer(StopAimingTimeHandle);
	}
	else
	{
		ShootingComponent->StopShooting();

		//Give some time to Stop Aiming so you can start aiming before shooting
		GetWorld()->GetTimerManager().SetTimer(StopAimingTimeHandle, [&]() {ShootingComponent->StopAiming(); }, 1.f, false);
	}
}

void AMShooterCharacter::Aim()
{
	if (!ShootingComponent)
	{
		return;
	}

	RequestNearestEnemyDelegate.Broadcast(this);
}

void AMShooterCharacter::GetNearestEnemy(AActor* NearestEnemy)
{
	if (!ShootingComponent)
	{
		return;
	}
	if (NearestEnemy)
	{
		ShootingComponent->StartAiming(NearestEnemy);
	}
	else
	{
		ShootingComponent->StopAiming();
	}
	
}

AActor* AMShooterCharacter::RequestGetAimedEnemy()
{
	if (!ShootingComponent)
	{
		return nullptr;
	}
	
	return ShootingComponent->GetCurrentAimedEnemy();
}