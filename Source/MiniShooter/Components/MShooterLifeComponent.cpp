// Fill out your copyright notice in the Description page of Project Settings.


#include "MShooterLifeComponent.h"

#include <Kismet/GameplayStatics.h>

#include "../MiniShooterGameMode.h"


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

	//Set Current Health variable to Max Health as a default initialization point
	Health = MaxHealth;
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
	//Determine if the NewHealth would be valid
	if (FinalHealth > 0 && FinalHealth <= MaxHealth)
	{
		//If it's valid then set the current health to that value
		Health = FinalHealth;
	}
	else if (FinalHealth <= 0)
	{
		//If Health is 0, then Kill Owning Actor
		Health = 0;
		KillOwner();
	}
}

void UMShooterLifeComponent::KillOwner()
{
	if (ensureMsgf(IsValid(GetOwner()), TEXT("%s faced error at Runtime"), *GetClass()->GetName()))
	{
		if (AMiniShooterGameMode* GameMode = Cast<AMiniShooterGameMode>(UGameplayStatics::GetGameMode(GetOwner()->GetWorld())))
		{
			//Notify GameMode if this Actor is about to die
			//Currently used mainly for Enemies and Shooting Targets, since GameMode keeps active track of them
			GameMode->NotifyEnemyDeadDelegate.Broadcast(GetOwner());
		}
		//Destroy Owner
		GetOwner()->Destroy();
	}
}
