// Fill out your copyright notice in the Description page of Project Settings.


#include "MShooterAIComponent.h"

#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/Character.h>

#include "../Enemy/MShooterEnemy.h"
#include "MShooterPatrolZone.h"

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

	MoveActor(DeltaTime);
}

void UMShooterAIComponent::ChangeState(EStateMachine NewState)
{
	if (!GetOwner())
	{
		return;
	}

	//Switch for functionality
	switch (NewState)
	{
		case EStateMachine::Idle:

			break;
		case EStateMachine::Patrol:
			StartPatrolFunctionality();
			GetOwner()->GetWorld()->GetTimerManager().SetTimer(InSameStateTimer, [&]() {StartRestFunctionality(); }, TimeBeforeResting, false);
			break;
		case EStateMachine::Chase:
			StartChaseFunctionality();
			GetOwner()->GetWorld()->GetTimerManager().SetTimer(InSameStateTimer, [&]() {StartRestFunctionality(); }, TimeBeforeResting, false);

			break;
		case EStateMachine::Rest:

			break;
		case EStateMachine::Attack:
			GetOwner()->GetWorld()->GetTimerManager().SetTimer(InSameStateTimer, [&]() {StartRestFunctionality(); }, TimeBeforeResting, false);
			break;
	}
	PreviousState = ActiveState;
	ActiveState = NewState;
}

void UMShooterAIComponent::StartRestFunctionality()
{
	ChangeState(EStateMachine::Rest);

	const float TimeToResumePrevState = TimeOfResting;
	GetOwner()->GetWorld()->GetTimerManager().SetTimer(InSameStateTimer, [&]() {ChangeState(PreviousState); }, TimeToResumePrevState, false);
}

void UMShooterAIComponent::StartPatrolFunctionality()
{
	if (!GetOwner())
	{
		return;
	}

	if (AMShooterEnemy* Enemy = Cast<AMShooterEnemy>(GetOwner()))
	{
		if (AMShooterPatrolZone* PatrolZone = Cast<AMShooterPatrolZone>(Enemy->GetActivePatrolZone()))
		{
			//This handles getting a new target location and add it it to the local variable TargetLocation
			PatrolZone->SendPatrolPointDelegate.Broadcast(GetOwner());
		}
	}
}

void UMShooterAIComponent::StartChaseFunctionality()
{
	if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetOwner()->GetWorld(), 0))
	{
		TargetLocation = Player->GetActorLocation();
	}
	
}

void UMShooterAIComponent::MoveActor(float DeltaTime)
{
	if (!GetOwner() || (ActiveState != EStateMachine::Patrol && ActiveState != EStateMachine::Chase))
	{
		return;
	}

	//Deal with possible vertical issues
	FHitResult HitResult;
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	const float TraceVerticalOffset = 400.f;
	FVector FloorLocation = CurrentLocation - (GetOwner()->GetActorUpVector() * TraceVerticalOffset);
	FCollisionQueryParams Collision;
	Collision.AddIgnoredActor(GetOwner());
	GetOwner()->GetWorld()->LineTraceSingleByChannel(HitResult, CurrentLocation, FloorLocation, ECollisionChannel::ECC_Visibility,Collision);

	if (HitResult.GetActor())
	{
		const float Min = 20.f;
		if (HitResult.Distance < Min)
		{
			FVector CorrectedHeight = CurrentLocation + (GetOwner()->GetActorUpVector() * Min);
			GetOwner()->SetActorLocation(CorrectedHeight, true);
		}
	}

	//Move Forward
	CurrentLocation = GetOwner()->GetActorLocation();
	FVector NewLocation;

	if (ActiveState == EStateMachine::Chase)
	{
		if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetOwner()->GetWorld(), 0))
		{
			TargetLocation = Player->GetActorLocation();
		}
		const float Speed = 2 / ChaseRunSpeed;
		const float AlphaTime = FMath::Clamp(DeltaTime / Speed, 0.0f, 1.0f);
		NewLocation = UKismetMathLibrary::VLerp(CurrentLocation, TargetLocation, AlphaTime);
	}
	else
	{
		const float Speed = 2 / PatrolWalkSpeed;
		const float AlphaTime = FMath::Clamp(DeltaTime / Speed, 0.0f, 1.0f);
		NewLocation = UKismetMathLibrary::VLerp(CurrentLocation, TargetLocation, AlphaTime);
	}

	GetOwner()->SetActorLocation(NewLocation, true);

	if (GetOwner()->GetActorLocation().Equals(TargetLocation, 50.f))
	{
		if (AMShooterEnemy* Enemy = Cast<AMShooterEnemy>(GetOwner()))
		{
			if (AMShooterPatrolZone* PatrolZone = Cast<AMShooterPatrolZone>(Enemy->GetActivePatrolZone()))
			{
				//This handles getting a new target location and add it it to the local variable TargetLocation
				PatrolZone->SendPatrolPointDelegate.Broadcast(GetOwner());
			}
		}
	}
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
	if (bActivate)
	{
		ChangeState(EStateMachine::Patrol);
	}
	else
	{
		ChangeState(EStateMachine::Idle);
	}
}