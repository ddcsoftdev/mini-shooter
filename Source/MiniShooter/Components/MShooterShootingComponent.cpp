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

	AimingRotationAdjustment();
}

void UMShooterShootingComponent::StartShooting()
{
	if (!GetWorld())
	{
		return;
	}

	if (GetWorld()->GetTimerManager().IsTimerActive(ShootingTimer))
	{
		return;
	}
	bIsShooting = true;
	TriggerShot();
	GetWorld()->GetTimerManager().SetTimer(ShootingTimer, this, &UMShooterShootingComponent::TriggerShot, 1/ ShootingSpeed, true);
}

void UMShooterShootingComponent::StopShooting()
{
	if (!GetWorld())
	{
		return;
	}

	if (!GetWorld()->GetTimerManager().IsTimerActive(ShootingTimer))
	{
		return;
	}
	bIsShooting = false;
	GetWorld()->GetTimerManager().ClearTimer(ShootingTimer);
}

void UMShooterShootingComponent::TriggerShot()
{
	if (!ProjectileBPReference || !GetOwner())
	{
		return;
	}

	//This is used to adjust location spawn to crosshair
	const float CrosshairVerticalOffset = 80.f;
	const FVector SpawnLocation = GetOwner()->GetActorLocation() + (FVector(0.f, 0.f, CrosshairVerticalOffset) + GetOwner()->GetActorForwardVector() * 10.f);
	const FActorSpawnParameters SpawnParams = FActorSpawnParameters();
	FRotator SpawnRotation = GetOwner()->GetActorRotation();

	//Scalable: If this is player, spawn according to camera 
	if (AMShooterCharacter* Player = Cast<AMShooterCharacter>(GetOwner()))
	{
		if (UCameraComponent* Camera = Player->GetFollowCamera())
		{
			SpawnRotation = Camera->GetComponentRotation();
		}
	}

	AMShooterProjectile* Projectile = GetOwner()->GetWorld()->SpawnActor<AMShooterProjectile>(ProjectileBPReference, SpawnLocation, SpawnRotation, SpawnParams);
	if (Projectile)
	{
		Projectile->RegisterProjectileOwner(GetOwner());
		if (bOverrideProjectileSpeed)
		{
			Projectile->OverrideProjectileSpeed(ProjectileSpeed);
		}
	}
}

void UMShooterShootingComponent::StartAiming(AActor* NearestEnemy)
{
	if (!NearestEnemy || !GetOwner())
	{
		return;
	}
	CurrentAimedEnemy = NearestEnemy;
	bIsAiming = true;
}

void UMShooterShootingComponent::StopAiming()
{
	CurrentAimedEnemy = nullptr;
	bIsAiming = false;
}

void UMShooterShootingComponent::AimingRotationAdjustment()
{
	if (!bIsAiming || !CurrentAimedEnemy || !GetOwner())
	{
		return;
	}

	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), CurrentAimedEnemy->GetActorLocation());
	const float GetActorVerticalRotation = GetOwner()->GetActorRotation().Roll;
	GetOwner()->SetActorRotation(FRotator(GetOwner()->GetActorRotation().Pitch, NewRotation.Yaw, NewRotation.Roll));

	//Scalable: if player rotate camera also. Check if player by casting to class
	if (AMShooterCharacter* Player = Cast<AMShooterCharacter>(GetOwner()))
	{
		AController* Controller = Player->GetController();
		UCameraComponent* Camera = Player->GetFollowCamera();
		if (Controller && Camera)
		{
			const float InterSpeed = 10.f;
			FRotator CameraRotation = UKismetMathLibrary::FindLookAtRotation(Camera->GetComponentLocation(), CurrentAimedEnemy->GetActorLocation());
			FRotator InterpRotation = UKismetMathLibrary::RInterpTo(Controller->GetControlRotation(), CameraRotation, GetWorld()->GetDeltaSeconds(), InterSpeed);
			FRotator NewControlRotation = FRotator(Controller->GetControlRotation().Pitch, InterpRotation.Yaw, Controller->GetControlRotation().Roll);
			Controller->SetControlRotation(NewControlRotation);
		}

	}
}
