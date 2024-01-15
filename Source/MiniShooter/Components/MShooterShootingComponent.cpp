// Fill out your copyright notice in the Description page of Project Settings.


#include "MShooterShootingComponent.h"

#include "../Projectile/MShooterProjectile.h"

// Sets default values for this component's properties
UMShooterShootingComponent::UMShooterShootingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMShooterShootingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMShooterShootingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMShooterShootingComponent::TriggerShot()
{

	//ConstructorHelpers::FClassFinder<AActor> BPFinder(TEXT("/Game/Path/To/BP"));
	//TSubclassOf<AActor> BPToSpawn; = BPFinder.Class;
	//AActor* Projectile = GetWorld()->SpawnActor(BPToSpawn, ...);
	AActor* Projectile = nullptr;
	if (bOverrideProjectileSpeed)
	{
		if (AMShooterProjectile* CastedProjectile = Cast<AMShooterProjectile>(Projectile))
		{
			CastedProjectile->OverrideProjectileSpeed(ProjectileSpeed);
		}
	}

}

