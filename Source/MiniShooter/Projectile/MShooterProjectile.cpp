// Fill out your copyright notice in the Description page of Project Settings.


#include "MShooterProjectile.h"

#include <Kismet/KismetMathLibrary.h>

// Sets default values
AMShooterProjectile::AMShooterProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMShooterProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMShooterProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveProjectile(DeltaTime);
}

void AMShooterProjectile::OverrideProjectileSpeed(float OverrideProjectileSpeed)
{
	ProjectileSpeed = OverrideProjectileSpeed;
}

void AMShooterProjectile::MoveProjectile(float DeltaTime)
{
	FVector EndLocation = GetActorForwardVector() * 5000.f;
	UKismetMathLibrary::VInterpTo(GetActorLocation(), EndLocation, DeltaTime, ProjectileSpeed);
}

