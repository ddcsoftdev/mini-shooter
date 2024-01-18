// Fill out your copyright notice in the Description page of Project Settings.


#include "MShooterProjectile.h"

#include <Kismet/KismetMathLibrary.h>
#include <Components/StaticMeshComponent.h>

#include "../Enemy/MShooterEnemy.h"
#include "../Enemy/MShooterTarget.h"
#include "../AI/MShooterPatrolZone.h"

// Sets default values
AMShooterProjectile::AMShooterProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create Static Mesh Component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("StaticMesh")));
	StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	SetRootComponent(StaticMeshComponent);
}

// Called when the game starts or when spawned
void AMShooterProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	//Start a time to destroy in case Projectile does not interact and gets lost in the void
	//This is quick replace for Kill beyond coordinates
	StartDestroyTimer();
}

// Called every frame
void AMShooterProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Handles moving the Projectile every frame in a straight line
	MoveProjectile(DeltaTime);
}

void AMShooterProjectile::OverrideProjectileSpeed(float OverrideProjectileSpeed)
{
	ProjectileSpeed = OverrideProjectileSpeed;
}

void AMShooterProjectile::MoveProjectile(float DeltaTime)
{
	//Primitive way to move Projectile without knowing ending location
	//No LineTrace for this simple interaction
	FVector EndLocation = GetActorLocation() + (GetActorForwardVector() * ProjectileSpeed);
	SetActorLocation(EndLocation, true);
}

void AMShooterProjectile::StartDestroyTimer()
{
	//Schedule up FTimerHandle for Destroy
	if (ensureMsgf(IsValid(GetWorld()), TEXT("%s faced error at Runtime"), *GetClass()->GetName()))
	{
		FTimerDelegate DestroyDelegate;
		DestroyDelegate.BindUFunction(this, "RequestProjectileDestroy");
		GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, DestroyDelegate, TimeToDestroy, false);
	}
}

void AMShooterProjectile::RequestProjectileDestroy()
{
	Destroy();
}

void AMShooterProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (ensureMsgf(IsValid(RegisteredOwnerOfProjectile), TEXT("%s faced error at Runtime"), *GetClass()->GetName()))
	{
		//Safeguard against any possible contact with Owner
		if (!IsValid(OtherActor) || OtherActor == RegisteredOwnerOfProjectile)
		{
			return;
		}
	}


	if (OtherActor->GetClass()->GetSuperClass() == AMShooterProjectile::StaticClass())
	{
		//Ignore other bullets as per design and do NOT destroy
		return;
	}
	else if (OtherActor->GetClass()->GetSuperClass() == AMShooterPatrolZone::StaticClass())
	{
		//Ignore patrol zone collision and do NOT destroy
		return;
	}

	if (OtherActor->GetClass()->GetSuperClass() == AMShooterEnemy::StaticClass())
	{
		//If Enemy deal damage
		Cast<AMShooterEnemy>(OtherActor)->TakeDamageAmount(ProjectileDamage);
	}
	else if (OtherActor->GetClass()->GetSuperClass() == AMShooterTarget::StaticClass())
	{
		//If Target deal damage
		Cast<AMShooterTarget>(OtherActor)->TakeDamageAmount(ProjectileDamage);
	}

	//Destroy bullet upon valid collision
	RequestProjectileDestroy();
}

void AMShooterProjectile::RegisterProjectileOwner(AActor* RegisterOwner)
{
	//Registering Owner and Enabling Collision
	RegisteredOwnerOfProjectile = RegisterOwner;
	StaticMeshComponent->SetCollisionProfileName(TEXT("OverlapAll"));
}

