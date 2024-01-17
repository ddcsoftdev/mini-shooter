// Fill out your copyright notice in the Description page of Project Settings.


#include "MShooterPatrolZone.h"

#include <Kismet/GameplayStatics.h>

#include "MShooterPatrolPoint.h"
#include "../Enemy/MShooterEnemy.h"
#include "../Player/MShooterCharacter.h"

// Sets default values
AMShooterPatrolZone::AMShooterPatrolZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMShooterPatrolZone::BeginPlay()
{
	Super::BeginPlay();
	
	//Bind Delegate to send new Patrol Points to Enemies within the Zone
	SendPatrolPointDelegate.AddUObject(this, &AMShooterPatrolZone::SendPatrolPoint);

	if (ensureMsgf(GetWorld(), TEXT("%s couldn't load %s at Runtime"), *GetClass()->GetName(), *GetWorld()->GetClass()->GetName()))
	{
		//Initialize by Registering all Overlapping Actors and starting AI in relevant Enemies
		//Delayed with a TimerHandle to allow all Actors to properly Spawn within level before attemping scan
		FTimerHandle InitializeTimerHander;
		GetWorld()->GetTimerManager().SetTimer(InitializeTimerHander, [&]() {Initialize(); }, 0.25f, false);
	}
}

// Called every frame
void AMShooterPatrolZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMShooterPatrolZone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	if (OtherActor->GetClass()->GetSuperClass() == AMShooterPatrolPoint::StaticClass())
	{
		//Registering Patrol Point when found
		RegisterPatrolPoint(OtherActor);
	}
	else if (OtherActor->GetClass()->GetSuperClass() == AMShooterEnemy::StaticClass())
	{
		//Registering Enemy when found
		RegisterEnemy(OtherActor);
	}
	else if (OtherActor->GetClass()->GetSuperClass() == AMShooterCharacter::StaticClass())
	{
		//Alert all Enemies that Player has entered this Patrol Zone
		PlayerIsInsideZoneDelegate.Broadcast(true);
		bIsPlayerInZone = true;
	}
}

void AMShooterPatrolZone::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	if (OtherActor->GetClass()->GetSuperClass() == AMShooterPatrolPoint::StaticClass())
	{
		//Unregister Patrol Point if it ends overlap with Patrol Zone
		UnRegisterPatrolPoint(OtherActor);
	}
	else if (OtherActor->GetClass()->GetSuperClass() == AMShooterEnemy::StaticClass())
	{
		//Unregister Enemy if it ends overlap with Patrol Zone
		UnRegisterEnemy(OtherActor);
	}
	else if (OtherActor->GetClass()->GetSuperClass() == AMShooterCharacter::StaticClass())
	{
		//Alert all Enemies that Player has exited this Patrol Zone
		PlayerIsInsideZoneDelegate.Broadcast(false);
		bIsPlayerInZone = false;
	}
}

void AMShooterPatrolZone::SendPatrolPoint(AActor* EnemyToSend)
{
	//As a safety meassure check that Player is not within Zone before sending a new Patrol Point
	if (bIsPlayerInZone)
	{
		//If it is Notify Enemy and do not send a Patrol Point
		PlayerIsInsideZoneDelegate.Broadcast(true);
		return;
	}

	if (IsValid(EnemyToSend))
	{
		//Get a valid Patrol Point from TArray
		if (AActor* PatrolPoint = GetAvailablePatrolPoint())
		{
			if (AMShooterEnemy* Enemy = Cast<AMShooterEnemy>(EnemyToSend))
			{
				//Send the Enemy the validated new Patrol Point
				Enemy->UpdateAITargetLocation(PatrolPoint);
				//Unregister the sent Patrol Point from TArray
				UnRegisterPatrolPoint(PatrolPoint);
			}
		}
	}
}

void AMShooterPatrolZone::RegisterPatrolPoint(AActor* PatrolPoint)
{
	//Put Patrol Point into TArray
	AvailablePatrolPoints.AddUnique(PatrolPoint);
}

void AMShooterPatrolZone::RegisterEnemy(AActor* Enemy)
{
	//Put Enemy into TArray
	ActiveEnemies.AddUnique(Enemy);
	if (AMShooterEnemy* CastedEnemy = Cast<AMShooterEnemy>(Enemy))
	{
		//Telling Enemy to register this as it's Patrol Zone
		CastedEnemy->RegisterPatrolZone(this);
		//Start AI Behaviour for Enemy
		CastedEnemy->SetAIBehaviour(true);
	}
}

void AMShooterPatrolZone::UnRegisterPatrolPoint(AActor* PatrolPoint)
{
	//Take Patrol Point out of TArray
	AvailablePatrolPoints.RemoveSingle(PatrolPoint);
}

void AMShooterPatrolZone::UnRegisterEnemy(AActor* Enemy)
{
	//Take Enemy out of TArray
	ActiveEnemies.RemoveSingle(Enemy);
}

void AMShooterPatrolZone::NotifyPlayerEnteredZone(AActor* Enemy)
{
	PlayerIsInsideZoneDelegate.Broadcast(true);
}

void AMShooterPatrolZone::NotifyPlayerLeftZone(AActor* Enemy)
{
	PlayerIsInsideZoneDelegate.Broadcast(false);
}

AActor* AMShooterPatrolZone::GetAvailablePatrolPoint()
{
	if (AvailablePatrolPoints.Num() == 0)
	{
		return nullptr;
	}
	//Get valid random Patrol Poing for TArray
	int32 Seed = FMath::RandRange(0, AvailablePatrolPoints.Num() - 1);
	return AvailablePatrolPoints[Seed];
}

void AMShooterPatrolZone::TryToReturnPatrolPoint(AActor* PatrolPoint)
{
	RegisterPatrolPoint(PatrolPoint);
}

void AMShooterPatrolZone::Initialize()
{
	//Scan all overlapping Actors
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	//Try to get all Patrol Points first, so they can be passed to Enemy upon registry
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor->GetClass()->GetSuperClass() == AMShooterPatrolPoint::StaticClass())
		{
			RegisterPatrolPoint(Actor);
		}
	}
	//Scan for Enemies, and also Player if it happens to spawn within Patrol Zone
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor->GetClass()->GetSuperClass() == AMShooterEnemy::StaticClass())
		{
			RegisterEnemy(Actor);
		}
		else if (Actor->GetClass()->GetSuperClass() == AMiniShooterCharacter::StaticClass())
		{
			PlayerIsInsideZoneDelegate.Broadcast(true);
		}
	}
}

void AMShooterPatrolZone::CheckIfPlayerStillInZone(AActor* Enemy)
{
	if (IsValid(Enemy))
	{
		if (AMShooterEnemy* CastedEnemy = Cast<AMShooterEnemy>(Enemy))
		{
			if (bIsPlayerInZone)
			{
				PlayerIsInsideZoneDelegate.Broadcast(true);
			}
			else
			{
				PlayerIsInsideZoneDelegate.Broadcast(false);
			}
		}
	}
}