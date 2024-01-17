// Fill out your copyright notice in the Description page of Project Settings.


#include "MShooterController.h"

AMShooterController::AMShooterController()
{

}

void AMShooterController::BeginPlay()
{
	Super::BeginPlay();

}

void AMShooterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//Early return if InputComponent could not be loaded
	if (!ensureMsgf(InputComponent, TEXT("%s couldn't load %s at Runtime"), *GetClass()->GetName(), *InputComponent->GetClass()->GetName()))
	{
		return;
	}

	//Request Aim Binding. This state only ends when stop Shooting state ends as per desing.
	InputComponent->BindAction("Aim", IE_Pressed, this, &AMShooterController::RequestAim);

	/*Start Request Shooting Binding : Longer input binding to pass a Bool along the Delegate created by UE*/
	//Start Shooting State
	bool bStartShooting = true;

	FInputActionHandlerSignature StartShootingActionHandler;
	StartShootingActionHandler.BindUFunction(this, TEXT("RequestShoot"), bStartShooting);

	FInputActionBinding StartShootingActionBinding("Shoot", IE_Pressed);
	StartShootingActionBinding.ActionDelegate = StartShootingActionHandler;
	InputComponent->AddActionBinding(StartShootingActionBinding);

	//End Shooting State
	bStartShooting = false;

	FInputActionHandlerSignature StopShootingActionHandler;
	StopShootingActionHandler.BindUFunction(this, TEXT("RequestShoot"), bStartShooting);

	FInputActionBinding StopShootingActionBinding("Shoot", IE_Released);
	StopShootingActionBinding.ActionDelegate = StopShootingActionHandler;
	InputComponent->AddActionBinding(StopShootingActionBinding);
	/*End Request Shooting Binding*/
}

void AMShooterController::RequestShoot(bool bStart)
{
	//Broadcast ShootDelegate To Character with bool as arg
	if (ShootDelegate.IsBound())
	{
		ShootDelegate.Broadcast(bStart);
	}
}

void AMShooterController::RequestAim()
{
	//Broadcast AimDelegate To Character for state to start (end depends on Shooting as per design)
	if (AimDelegate.IsBound())
	{
		AimDelegate.Broadcast();
	}
}
