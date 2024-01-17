// Fill out your copyright notice in the Description page of Project Settings.


#include "MShooterPatrolPoint.h"

// Sets default values
AMShooterPatrolPoint::AMShooterPatrolPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMShooterPatrolPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMShooterPatrolPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

