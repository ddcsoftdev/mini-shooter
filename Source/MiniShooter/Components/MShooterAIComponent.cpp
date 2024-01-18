// Fill out your copyright notice in the Description page of Project Settings.


#include "MShooterAIComponent.h"

#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/Character.h>

#include "../Enemy/MShooterEnemy.h"
#include "../AI/MShooterPatrolZone.h"

// Sets default values for this component's properties
UMShooterAIComponent::UMShooterAIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UMShooterAIComponent::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void UMShooterAIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Move Actor to desired location
	MoveActor(DeltaTime);
}

void UMShooterAIComponent::ChangeState(EStateMachine NewState)
{
	if (!ensureMsgf(GetOwner() && GetWorld(), TEXT("%s couldn't load %s or %s at Runtime"), *GetClass()->GetName(), *GetOwner()->GetClass()->GetName(), *GetWorld()->GetClass()->GetName()))
	{
		return;
	}

	//AI Functionality dependant on the State Machine
	switch (NewState)
	{
		case EStateMachine::Idle:
			//Do nothing on Idle
			break;
		case EStateMachine::Patrol:
			//Patrol between Patrol Point within the active Patrol Zone
			StartPatrolFunctionality();
			//Set TimerHandle to transition to Rest State as per design
			GetWorld()->GetTimerManager().SetTimer(InSameStateTimer, [&]() {StartRestFunctionality(); }, TimeBeforeResting, false);
			break;
		case EStateMachine::Chase:
			//Chase Player within Patrol Zone
			StartChaseFunctionality();
			//Set TimerHandle to transition to Rest State as per design
			GetWorld()->GetTimerManager().SetTimer(InSameStateTimer, [&]() {StartRestFunctionality(); }, TimeBeforeResting, false);
			break;
		case EStateMachine::Rest:
			//Rest functionality is called with the Timer
			//Rests for some time and then returns to previous state
			break;
		case EStateMachine::Attack:
			//No current functionality added for this state
			break;
	}

	//Keeping track of previous state as a reference point
	PreviousState = ActiveState;
	ActiveState = NewState;
}

void UMShooterAIComponent::StartRestFunctionality()
{
	if (ensureMsgf(GetWorld(), TEXT("%s couldn't load %s at Runtime"), *GetClass()->GetName(), *GetWorld()->GetClass()->GetName()))
	{
		//This is called mainly to change the Enum since functionality resides here
		ChangeState(EStateMachine::Rest);
		//Set time for resting before returning to previous state
		const float TimeToResumePrevState = TimeOfResting;
		GetWorld()->GetTimerManager().SetTimer(InSameStateTimer, [&]() {ChangeState(PreviousState); }, TimeToResumePrevState, false);
	}
}

void UMShooterAIComponent::StartPatrolFunctionality()
{
	if (ensureMsgf(GetOwner(), TEXT("%s couldn't load %s at Runtime"), *GetClass()->GetName(), *GetOwner()->GetClass()->GetName()))
	{
		if (AMShooterEnemy* Enemy = Cast<AMShooterEnemy>(GetOwner()))
		{
			if (AMShooterPatrolZone* PatrolZone = Cast<AMShooterPatrolZone>(Enemy->GetActivePatrolZone()))
			{
				//Asks Patrol Zone Delegate to send a new Patrol Point to reference it as the Actor's next Location
				PatrolZone->SendPatrolPointDelegate.Broadcast(GetOwner());
			}
		}
	}
}

void UMShooterAIComponent::StartChaseFunctionality()
{
	if (ensureMsgf(GetWorld(), TEXT("%s couldn't load %s at Runtime"), *GetClass()->GetName(), *GetWorld()->GetClass()->GetName()))
	{
		if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		{
			//Get Player and set it as it's new Location, this location will update every frame within MoveActor
			TargetLocation = Player->GetActorLocation();
		}
	}
	else
	{
		ChangeState(EStateMachine::Chase);
	}
}

void UMShooterAIComponent::MoveActor(float DeltaTime)
{
	//Actor only moves on Patrol and Chase so return on rest
	if (ActiveState != EStateMachine::Patrol && ActiveState != EStateMachine::Chase)
	{
		return;
	}
	if (!ensureMsgf(GetOwner() && GetWorld(), TEXT("%s couldn't load %s at Runtime"), *GetClass()->GetName(), *GetOwner()->GetClass()->GetName() , *GetWorld()->GetClass()->GetName()))
	{
		return;
	}

	//Try to avoid vertical issues where Actor is Spawned too close to the floor, and thus it get's stucked
	//Also can work for stairs and level changes
	FHitResult HitResult;
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	const float TraceVerticalOffset = 400.f;
	FVector FloorLocation = CurrentLocation - (GetOwner()->GetActorUpVector() * TraceVerticalOffset);
	FCollisionQueryParams Collision;
	Collision.AddIgnoredActor(GetOwner());
	//Do a trace looking for the first object below (normally platform on which Actor is standing)
	GetWorld()->LineTraceSingleByChannel(HitResult, CurrentLocation, FloorLocation, ECollisionChannel::ECC_Visibility,Collision);
	//Apply changes to vertical vector only
	if (IsValid(HitResult.GetActor()))
	{
		const float Min = 50.f;
		//If distance to floor is less that Min, then add Min as height to current Actor Z vector
		if (HitResult.Distance < Min)
		{
			//Set Actor Vertical Location
			FVector CorrectedHeight = CurrentLocation + (GetOwner()->GetActorUpVector() * Min);
			GetOwner()->SetActorLocation(CorrectedHeight, true);
		}
	}

	//This is used when checking if arrived at location, for Patrol is smaller than Chase to avoid Enemies bashing into each other
	float ErrorPerctengateToLocation = 50.f;
	if (ActiveState == EStateMachine::Chase)
	{
		//Adjusting this variable to avoid bashing between Enemies
		ErrorPerctengateToLocation = 300.f;
	}

	//Check to verify if Actor has reached TargetLocation
	if (GetOwner()->GetActorLocation().Equals(TargetLocation, ErrorPerctengateToLocation))
	{

		if (AMShooterEnemy* Enemy = Cast<AMShooterEnemy>(GetOwner()))
		{
			if (AMShooterPatrolZone* PatrolZone = Cast<AMShooterPatrolZone>(Enemy->GetActivePatrolZone()))
			{
				if (ActiveState == EStateMachine::Chase)
				{
					//Make sure that Enemy should still follow Player as a failsaife
					PatrolZone->CheckIfPlayerStillInZone(GetOwner());
				}
				else
				{
					//This handles getting a new target location and add it it to the local variable TargetLocation
					PatrolZone->SendPatrolPointDelegate.Broadcast(GetOwner());
				}

			}
		}
		//return to avoid Moving actor since it has arrived to location
		return;
	}

	//Move Forward based on Current Location (which may be altered vertically)
	CurrentLocation = GetOwner()->GetActorLocation();
	FVector NewLocation;

	if (ActiveState == EStateMachine::Chase)
	{
		//If in Chase state use for NewLocation the Player Location and update this location each framse
		if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		{
			TargetLocation = Player->GetActorLocation();
		}
		//Apply Lerp with the current ChaseRunSpeed variable
		const float Speed = 2 / ChaseRunSpeed;
		const float AlphaTime = FMath::Clamp(DeltaTime / Speed, 0.0f, 1.0f);
		NewLocation = UKismetMathLibrary::VLerp(CurrentLocation, TargetLocation, AlphaTime);
	}
	else
	{
		//Patrol does not need to update Location each frame as it takes the static location from the Patrol Point
		//Apply Lerp with PatrolWalkSpeed
		const float Speed = 2 / PatrolWalkSpeed;
		const float AlphaTime = FMath::Clamp(DeltaTime / Speed, 0.0f, 1.0f);
		NewLocation = UKismetMathLibrary::VLerp(CurrentLocation, TargetLocation, AlphaTime);
	}
	//Set Actor Forward Movement after checking that is safe to move
	GetOwner()->SetActorLocation(NewLocation, true);
}

void UMShooterAIComponent::StartPlayerChase()
{
	ChangeState(EStateMachine::Chase);
}

void UMShooterAIComponent::StopPlayerChase()
{
	ChangeState(EStateMachine::Patrol);
}

void UMShooterAIComponent::SetTargetLocation(FVector NewLocation)
{
	TargetLocation = NewLocation;
}

void UMShooterAIComponent::SetAIBehaviour(bool bActivate)
{
	//Activates AI by Kickstarting Patrol, or Deactivates by switching to Idle
	if (bActivate)
	{
		ChangeState(EStateMachine::Patrol);
	}
	else
	{
		ChangeState(EStateMachine::Idle);
	}
}