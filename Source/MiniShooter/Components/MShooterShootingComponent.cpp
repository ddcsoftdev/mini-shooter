// Fill out your copyright notice in the Description page of Project Settings.


#include "MShooterShootingComponent.h"

#include "../Projectile/MShooterProjectile.h"

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

	// ...
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
	TriggerShot();
	GetWorld()->GetTimerManager().SetTimer(ShootingTimer, this, &UMShooterShootingComponent::TriggerShot, ShootingCadence, true);
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

	GetWorld()->GetTimerManager().ClearTimer(ShootingTimer);
}

void UMShooterShootingComponent::TriggerShot()
{
	if (!ProjectileBPReference || !GetOwner())
	{
		return;
	}

	const FVector SpawnLocation = GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector() * 10.f);
	const FRotator SpawnRotation = GetOwner()->GetActorRotation();
	const FActorSpawnParameters SpawnParams = FActorSpawnParameters();

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

