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
	
	SendPatrolPointDelegate.AddUObject(this, &AMShooterPatrolZone::SendPatrolPoint);

	//Delay Initialization so All Actors load properly
	FTimerHandle InitializeTimerHander;
	GetWorld()->GetTimerManager().SetTimer(InitializeTimerHander, [&]() {Initialize(); }, 0.25f, false);
}

// Called every frame
void AMShooterPatrolZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMShooterPatrolZone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->GetClass()->GetSuperClass() == AMShooterPatrolPoint::StaticClass())
	{
		RegisterPatrolPoint(OtherActor);
	}
	else if (OtherActor->GetClass()->GetSuperClass() == AMShooterEnemy::StaticClass())
	{
		RegisterEnemy(OtherActor);
	}
	else if (OtherActor->GetClass()->GetSuperClass() == AMShooterCharacter::StaticClass())
	{
		PlayerIsInsideZoneDelegate.Broadcast(true);
	}
}

void AMShooterPatrolZone::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (OtherActor->GetClass()->GetSuperClass() == AMShooterPatrolPoint::StaticClass())
	{
		UnRegisterPatrolPoint(OtherActor);
	}
	else if (OtherActor->GetClass()->GetSuperClass() == AMShooterEnemy::StaticClass())
	{
		UnRegisterEnemy(OtherActor);
	}
	else if (OtherActor->GetClass()->GetSuperClass() == AMShooterCharacter::StaticClass())
	{
		PlayerIsInsideZoneDelegate.Broadcast(false);
	}
}

void AMShooterPatrolZone::SendPatrolPoint(AActor* EnemyToSend)
{
	if (AActor* PatrolPoint = GetAvailablePatrolPoint())
	{
		if (AMShooterEnemy* Enemy = Cast<AMShooterEnemy>(EnemyToSend))
		{
			Enemy->UpdateAITargetLocation(PatrolPoint);
			UnRegisterPatrolPoint(PatrolPoint);
		}
	}
}

void AMShooterPatrolZone::RegisterPatrolPoint(AActor* PatrolPoint)
{
	//Avoids adding patrol point twice
	AvailablePatrolPoints.AddUnique(PatrolPoint);
}

void AMShooterPatrolZone::RegisterEnemy(AActor* Enemy)
{
	ActiveEnemies.AddUnique(Enemy);
	if (AMShooterEnemy* CastedEnemy = Cast<AMShooterEnemy>(Enemy))
	{
		CastedEnemy->RegisterPatrolZone(this);
		CastedEnemy->SetAIBehaviour(true);
	}
}

void AMShooterPatrolZone::UnRegisterPatrolPoint(AActor* PatrolPoint)
{
	AvailablePatrolPoints.RemoveSingle(PatrolPoint);
}

void AMShooterPatrolZone::UnRegisterEnemy(AActor* Enemy)
{
	ActiveEnemies.RemoveSingle(Enemy);
	if (AMShooterEnemy* CastedEnemy = Cast<AMShooterEnemy>(Enemy))
	{
		CastedEnemy->UnRegisterPatrolZone(this);
	}
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
	int32 Seed = FMath::RandRange(0, AvailablePatrolPoints.Num() - 1);

	return AvailablePatrolPoints[Seed];
}

void AMShooterPatrolZone::TryToReturnPatrolPoint(AActor* PatrolPoint)
{
	RegisterPatrolPoint(PatrolPoint);
}

void AMShooterPatrolZone::Initialize()
{
	//Do Initial Scan for Actors
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	//First Scan Patrol Points
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor->GetClass()->GetSuperClass() == AMShooterPatrolPoint::StaticClass())
		{
			RegisterPatrolPoint(Actor);
		}
	}
	//Then do rest
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