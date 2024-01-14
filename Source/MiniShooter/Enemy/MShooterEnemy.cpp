// Fill out your copyright notice in the Description page of Project Settings.


#include "MShooterEnemy.h"

#include <Components/StaticMeshComponent.h>
#include <Components/BoxComponent.h>

#include "../Components/MShooterLifeComponent.h"

// Sets default values
AMShooterEnemy::AMShooterEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create Life Component
	LifeComponent = CreateDefaultSubobject<UMShooterLifeComponent>(FName(TEXT("LifeComponent")));
	//Create Static Mesh Component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("StaticMesh")));
	//Create Box Collision Box
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(FName(TEXT("Collision")));
}

// Called when the game starts or when spawned
void AMShooterEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMShooterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

