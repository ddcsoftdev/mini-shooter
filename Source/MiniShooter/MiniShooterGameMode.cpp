// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiniShooterGameMode.h"

#include <UObject/ConstructorHelpers.h>
#include <Kismet/GameplayStatics.h>

#include "MiniShooterCharacter.h"
#include "Player/MShooterCharacter.h"
#include "Enemy/MShooterEnemy.h"
#include "Enemy/MShooterTarget.h"


AMiniShooterGameMode::AMiniShooterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AMiniShooterGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (AMShooterCharacter* Player = Cast<AMShooterCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		Player->RequestNearestEnemyDelegate.AddUObject(this, &AMiniShooterGameMode::ProcessRequestEnemyDelegate);
	}

	NotifyEnemyDeadDelegate.AddUObject(this, &AMiniShooterGameMode::RemoveDeadEnemyFromActiveEnemies);

	ScanAllActiveEnemies();
}

void AMiniShooterGameMode::ProcessRequestEnemyDelegate(AActor* RequestingActor)
{
	AActor* NearestEnemy = nullptr;

	for (AActor* Enemy : ActiveEnemies)
	{
		if (!NearestEnemy)
		{
			NearestEnemy = Enemy;
			continue;
		}

		float CurrentEnemyDistance = NearestEnemy->GetDistanceTo(RequestingActor);
		float ContenderEnemyDistance = Enemy->GetDistanceTo(RequestingActor);
		if (ContenderEnemyDistance < CurrentEnemyDistance)
		{
			NearestEnemy = Enemy;
		}
	}

	SendEnemyDelegate.Broadcast(NearestEnemy);
}

void AMiniShooterGameMode::ScanAllActiveEnemies()
{
	ActiveEnemies.Empty();
	UGameplayStatics::GetAllActorsOfClass(this, AMShooterEnemy::StaticClass(), ActiveEnemies);

	//Not sure if it was design to Scan targets too, but doing so
	TArray<AActor*> Targets;
	UGameplayStatics::GetAllActorsOfClass(this, AMShooterTarget::StaticClass(), Targets);

	ActiveEnemies += Targets;
}

void AMiniShooterGameMode::RemoveDeadEnemyFromActiveEnemies(AActor* DeadEnemy)
{
	ActiveEnemies.RemoveSingle(DeadEnemy);

	//If killed enemy was the Aimed Target, get another target if available
	if (AMShooterCharacter* Player = Cast<AMShooterCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		if (AActor* CurrentTarget = Player->RequestGetAimedEnemy())
		{
			if (CurrentTarget == DeadEnemy)
			{
				ProcessRequestEnemyDelegate(Player);
			}
		}
	}
}
