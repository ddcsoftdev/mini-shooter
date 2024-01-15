// Fill out your copyright notice in the Description page of Project Settings.


#include "MShooterController.h"

AMShooterController::AMShooterController()
{

}

void AMShooterController::BeginPlay()
{
	Super::BeginPlay();

	SetupInputComponent();
}

void AMShooterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!InputComponent)
	{
		return;
	}

	InputComponent->BindAction("Shoot", IE_Pressed, this, &AMShooterController::RequestShoot);
}

void AMShooterController::RequestShoot()
{
	if (ShootDelegate.IsBound())
	{
		ShootDelegate.Broadcast();
	}
}

