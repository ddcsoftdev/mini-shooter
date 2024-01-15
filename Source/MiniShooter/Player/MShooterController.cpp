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

	if (!InputComponent)
	{
		return;
	}
	
	/*Handle compound input bindings*/

	//Shooting Start
	FInputActionHandlerSignature StartShootingActionHandler;
	//Obj / Func name / Variable
	StartShootingActionHandler.BindUFunction(this, TEXT("RequestShoot"), true);

	FInputActionBinding StartShootingActionBinding("Shoot", IE_Pressed);
	StartShootingActionBinding.ActionDelegate = StartShootingActionHandler;
	InputComponent->AddActionBinding(StartShootingActionBinding);

	//Shooting End
	FInputActionHandlerSignature StopShootingActionHandler;
	//Obj / Func name / Variable
	StopShootingActionHandler.BindUFunction(this, TEXT("RequestShoot"), false);

	FInputActionBinding StopShootingActionBinding("Shoot", IE_Released);
	StopShootingActionBinding.ActionDelegate = StopShootingActionHandler;
	InputComponent->AddActionBinding(StopShootingActionBinding);

	/*handle compund input bindins end*/

}

void AMShooterController::RequestShoot(bool bStart)
{
	if (ShootDelegate.IsBound())
	{
		ShootDelegate.Broadcast(bStart);
	}
}

