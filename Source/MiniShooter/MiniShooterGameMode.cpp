// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiniShooterGameMode.h"

#include <UObject/ConstructorHelpers.h>
#include <Kismet/GameplayStatics.h>

#include "Player/MShooterCharacter.h"
#include "Enemy/MShooterEnemy.h"
#include "Enemy/MShooterTarget.h"
#include "UI/MShooterHUD.h"


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

	//Bind Delegates
	if (AMShooterCharacter* Player = Cast<AMShooterCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		Player->RequestNearestEnemyDelegate.AddUObject(this, &AMiniShooterGameMode::ProcessRequestEnemyDelegate);
	}
	NotifyEnemyDeadDelegate.AddUObject(this, &AMiniShooterGameMode::RemoveDeadEnemyFromActiveEnemies);

	//Scan All Enemies
	ScanAllActiveEnemies();
}

void AMiniShooterGameMode::ProcessRequestEnemyDelegate(AActor* RequestingActor)
{
	AActor* NearestEnemy = nullptr;
	AActor* CurrentlyAimedEnemy = nullptr;

	//Get Current Targeted enemy, and attempt to change to another
	//It still works accounting for second closest.
	if (AMShooterCharacter* Player = Cast<AMShooterCharacter>(RequestingActor))
	{
		CurrentlyAimedEnemy = Player->RequestGetAimedEnemy();
	}

	for (AActor* Enemy : ActiveEnemies)
	{
		if (!NearestEnemy)
		{
			NearestEnemy = Enemy;
			continue;
		}

		if (Enemy == CurrentlyAimedEnemy)
		{
			continue;
		}

		float CurrentEnemyDistance = NearestEnemy->GetDistanceTo(RequestingActor);
		float ContenderEnemyDistance = Enemy->GetDistanceTo(RequestingActor);
		if (ContenderEnemyDistance < CurrentEnemyDistance)
		{
			NearestEnemy = Enemy;
		}
	}
	//Send newly selected Enemy
	SendEnemyDelegate.Broadcast(NearestEnemy);
}

void AMiniShooterGameMode::ScanAllActiveEnemies()
{
	//Scan All Enemies, it does not count MShooterTarget as Enemies purposfully
	ActiveEnemies.Empty();
	UGameplayStatics::GetAllActorsOfClass(this, AMShooterEnemy::StaticClass(), ActiveEnemies);
}

void AMiniShooterGameMode::RemoveDeadEnemyFromActiveEnemies(AActor* DeadEnemy)
{
	//Remove Enemy from array when it notifies death
	ActiveEnemies.RemoveSingle(DeadEnemy);
	//Updated Killed Enemies Counter
	KilledEnemiesCount++;

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
	//Updating KillCount Widget
	if (ensureMsgf(GetWorld(), TEXT("%s couldn't load %s at Runtime"), *GetClass()->GetName(), *GetWorld()->GetClass()->GetName()))
	{
		
		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			if (AMShooterHUD* HUD = Cast<AMShooterHUD>(PlayerController->GetHUD()))
			{
				HUD->UpdateKillCountDelegate.Broadcast(KilledEnemiesCount);
			}
		}
	}

}
