// Fill out your copyright notice in the Description page of Project Settings.


#include "MShooterLifeComponent.h"

// Sets default values for this component's properties
UMShooterLifeComponent::UMShooterLifeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMShooterLifeComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UMShooterLifeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMShooterLifeComponent::SetHealth(float NewHealth)
{
	float FinalHealth = Health + NewHealth;
	if (FinalHealth > 0 && FinalHealth <= MaxHealth)
	{
		Health = FinalHealth;
	}
	else if (FinalHealth <= 0)
	{
		//DIE
		Health = 0;
		KillOwner();
	}
}

void UMShooterLifeComponent::KillOwner()
{
	if (GetOwner())
	{
		GetOwner()->Destroy();
	}
}
