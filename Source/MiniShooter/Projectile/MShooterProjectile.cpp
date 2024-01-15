// Fill out your copyright notice in the Description page of Project Settings.


#include "MShooterProjectile.h"

#include <Kismet/KismetMathLibrary.h>
#include <Components/StaticMeshComponent.h>

#include "../Enemy/MShooterEnemy.h"
#include "../Enemy/MShooterTarget.h"

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
	
	StartDestroyTimer();
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
	FVector EndLocation = GetActorLocation() + (GetActorForwardVector() * ProjectileSpeed);
	SetActorLocation(EndLocation, true);
}

void AMShooterProjectile::StartDestroyTimer()
{
	FTimerDelegate DestroyDelegate;
	DestroyDelegate.BindUFunction(this, "RequestProjectileDestroy");

	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, DestroyDelegate, TimeToDestroy, false);
}

void AMShooterProjectile::RequestProjectileDestroy()
{
	Destroy();
}

void AMShooterProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//Avoid owner to destroy bullet, specially if used with AI
	if (RegisteredOwnerOfProjectile && OtherActor == RegisteredOwnerOfProjectile)
	{
		return;
	}

	//Make sure other bullets do not affect this for crossfire
	if (OtherActor->GetClass()->GetSuperClass() == AMShooterProjectile::StaticClass())
	{
		return;
	}

	if (OtherActor->GetClass()->GetSuperClass() == AMShooterEnemy::StaticClass())
	{
		Cast<AMShooterEnemy>(OtherActor)->TakeDamageAmount(ProjectileDamage);
	}
	else if (OtherActor->GetClass()->GetSuperClass() == AMShooterTarget::StaticClass())
	{
		Cast<AMShooterTarget>(OtherActor)->TakeDamageAmount(ProjectileDamage);
	}
	RequestProjectileDestroy();
}

void AMShooterProjectile::RegisterProjectileOwner(AActor* RegisterOwner)
{
	RegisteredOwnerOfProjectile = RegisterOwner;
	StaticMeshComponent->SetCollisionProfileName(TEXT("OverlapAll"));
}

