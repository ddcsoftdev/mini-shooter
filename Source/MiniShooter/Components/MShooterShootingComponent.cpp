// Fill out your copyright notice in the Description page of Project Settings.


#include "MShooterShootingComponent.h"

#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Camera/CameraComponent.h>

#include "../Projectile/MShooterProjectile.h"
#include "../Player/MShooterCharacter.h"

// Sets default values for this component's properties
UMShooterShootingComponent::UMShooterShootingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Get BP Class for the Projectile in order to be able to Spawn it when Shooting
	ConstructorHelpers::FObjectFinder<UBlueprint> ProjectileFinder(TEXT("Blueprint'/Game/MiniShooter/Projectile/Projectile_BP.Projectile_BP'"));
	if (ProjectileFinder.Object)
	{
		ProjectileBPReference = (UClass*)ProjectileFinder.Object->GeneratedClass;
	}
}


// Called when the game starts
void UMShooterShootingComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UMShooterShootingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Rotates Camera (if Player) and Mesh to face active Target every frame
	AimingRotationAdjustment();
}

void UMShooterShootingComponent::StartShooting()
{
	if (ensureMsgf(GetWorld(), TEXT("%s couldn't load %s at Runtime"), *GetClass()->GetName(), *GetWorld()->GetClass()->GetName()))
	{
		//Set control bool variable to true
		bIsShooting = true;
		//Trigger shoot immediatly, since TimerHandle has a delay before it Trigger first shot
		TriggerShot();
		//Set TimerHandle to Trigger according to ShootingSpeed and make it loop until TimerHandle is cleared
		GetWorld()->GetTimerManager().SetTimer(ShootingTimer, this, &UMShooterShootingComponent::TriggerShot, 1 / ShootingSpeed, true);
	}
}

void UMShooterShootingComponent::StopShooting()
{
	if (ensureMsgf(GetWorld(), TEXT("%s couldn't load %s at Runtime"), *GetClass()->GetName(), *GetWorld()->GetClass()->GetName()))
	{
		//Set control bool variable to false
		bIsShooting = false;
		//Clear TimerHandle, breaking loop shooting as declared in StartShooting
		GetWorld()->GetTimerManager().ClearTimer(ShootingTimer);
	}
}

void UMShooterShootingComponent::RestartShootingTimer()
{
	if (ensureMsgf(GetWorld(), TEXT("%s couldn't load %s at Runtime"), *GetClass()->GetName(), *GetWorld()->GetClass()->GetName()))
	{
		GetWorld()->GetTimerManager().SetTimer(ShootingTimer, this, &UMShooterShootingComponent::TriggerShot, 1 / ShootingSpeed, true);
	}

}

void UMShooterShootingComponent::TriggerShot()
{
	if (!ensureMsgf(GetOwner() && ProjectileBPReference, TEXT("%s couldn't load %s or %s at Runtime"), *GetClass()->GetName(), *GetOwner()->GetClass()->GetName(), *ProjectileBPReference->GetClass()->GetName()))
	{
		return;
	}
	//Check as a failsafe control variable to make sure it's supposed to Shoot
	if (!bIsShooting)
	{
		return;
	}

	//Kickstar Spawning Parameters for properly Spawning Projectile
	FVector SpawnLocation = GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector() * 10.f);
	FRotator SpawnRotation = GetOwner()->GetActorRotation();
	const FActorSpawnParameters SpawnParams = FActorSpawnParameters();

	//If Actor is Player, offset Spawning Location so it aligns better to the HUD crosshair
	if (GetOwner()->GetClass()->GetSuperClass() == AMShooterCharacter::StaticClass())
	{
		//After testing, Vertical adjustment is needed for better alignment with crosshair
		const float CrosshairVerticalOffset = 80.f;
		SpawnLocation = GetOwner()->GetActorLocation() + (FVector(0.f, 0.f, CrosshairVerticalOffset) + GetOwner()->GetActorForwardVector() * 10.f);

		//If Actor is Player, make sure SpawnRotation is according to Player Camera so Spawn direction can be modified with Camera Rotation
		if (AMShooterCharacter* Player = Cast<AMShooterCharacter>(GetOwner()))
		{
			if (UCameraComponent* Camera = Player->GetFollowCamera())
			{
				SpawnRotation = Camera->GetComponentRotation();
			}
		}
	}

	//Spawn Projectile based on previous data
	AMShooterProjectile* Projectile = GetOwner()->GetWorld()->SpawnActor<AMShooterProjectile>(ProjectileBPReference, SpawnLocation, SpawnRotation, SpawnParams);
	//Check that Projectile has spawned correctly
	if (IsValid(Projectile))
	{
		//Register in the Projectile class this as it's Owner
		Projectile->RegisterProjectileOwner(GetOwner());
		//Check if it should override the default Projectile class speed
		if (bOverrideProjectileSpeed)
		{
			Projectile->OverrideProjectileSpeed(ProjectileSpeed);
		}
	}
}

void UMShooterShootingComponent::StartAiming(AActor* NearestEnemy)
{
	if (ensureMsgf(GetOwner(), TEXT("%s couldn't load %s at Runtime"), *GetClass()->GetName(), *GetOwner()->GetClass()->GetName()))
	{
		if (IsValid(NearestEnemy))
		{
			//Set control bool variable to true
			bIsAiming = true;
			//Set control AActor variable to point to the Nearest Enemy for Aiming
			CurrentAimedEnemy = NearestEnemy;
		}
	}
}

void UMShooterShootingComponent::StopAiming()
{
	// Set control bool variable to false
	bIsAiming = false;
	//Clean control AActor variable
	CurrentAimedEnemy = nullptr;
}

void UMShooterShootingComponent::AimingRotationAdjustment()
{
	//Since it's done in Tick, do not proceed if control variable bIsAiming is false
	//This is done first to avoid having to process ensure every Tick if not needed
	if (!bIsAiming)
	{
		return;
	}
	if (ensureMsgf(GetOwner() && CurrentAimedEnemy, TEXT("%s couldn't load %s at Runtime"), *GetClass()->GetName(), *GetOwner()->GetClass()->GetName()))
	{
		//Get Rotation Relative to the Target and set this Actor Rotation to face Target
		FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), CurrentAimedEnemy->GetActorLocation());
		const float GetActorVerticalRotation = GetOwner()->GetActorRotation().Roll;
		GetOwner()->SetActorRotation(FRotator(GetOwner()->GetActorRotation().Pitch, NewRotation.Yaw, NewRotation.Roll));

		//If Player is holding this Component, also rotate it's Camera to face Target
		if (GetOwner()->GetClass()->GetSuperClass() == AMShooterCharacter::StaticClass())
		{
			if (AMShooterCharacter* Player = Cast<AMShooterCharacter>(GetOwner()))
			{
				//Get Controller and Follow Camera
				AController* Controller = Player->GetController();
				UCameraComponent* Camera = Player->GetFollowCamera();
				if (IsValid(Controller) && IsValid(Camera))
				{
					//Set Interpolatio Speed for Camera Rotation
					const float InterSpeed = 10.f;
					//Calculate Rotation, by Interp you give flexibility for Player to adjust rotation a bit before this method corrects it back. Helps with aiming.
					FRotator CameraRotation = UKismetMathLibrary::FindLookAtRotation(Camera->GetComponentLocation(), CurrentAimedEnemy->GetActorLocation());
					FRotator InterpRotation = UKismetMathLibrary::RInterpTo(Controller->GetControlRotation(), CameraRotation, GetWorld()->GetDeltaSeconds(), InterSpeed);
					FRotator NewControlRotation = FRotator(Controller->GetControlRotation().Pitch, InterpRotation.Yaw, Controller->GetControlRotation().Roll);
					Controller->SetControlRotation(NewControlRotation);
				}
			}
		}
	}
}

