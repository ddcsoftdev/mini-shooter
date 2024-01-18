// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/GameModeBase.h>

#include "MiniShooterGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FSendNearestEnemy, class AActor*);
DECLARE_MULTICAST_DELEGATE_OneParam(FNotifyEnemyDead, class AActor*);

/**
* Reusing Default GameMode and adding custom functionality Mainly about Enemies and Aiming availability functionality
*/
UCLASS(minimalapi)
class AMiniShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMiniShooterGameMode();

	/**
	* Delegate that Player can use to get the nearest Enemy
	*/
	FSendNearestEnemy SendEnemyDelegate;

	/**
	* Delegate that is to be broadcasted when Enemy dies
	*/
	FNotifyEnemyDead NotifyEnemyDeadDelegate;

protected:

	/**
	* All active Enemies on map
	*/
	TArray<AActor*> ActiveEnemies;

	/**
	* Scan for all active enemies in map
	*/
	UFUNCTION()
		void ScanAllActiveEnemies();

	/**
	* Remove Dead Enemy from Active Enemies
 	*
  	* @param DeadEnemy Actor to remove from Enemy TArray
	*/
	UFUNCTION()
		void RemoveDeadEnemyFromActiveEnemies(AActor* DeadEnemy);

	/**
	* Method that processes RequestEnemyDelegate to send the nearest enemy
 	* New enemy must not be currently targeted
 	*
  	* @param RequestingActor Actor that is requesting the new Enemy
	*/
	UFUNCTION()
		void ProcessRequestEnemyDelegate(AActor* RequestingActor);

	virtual void BeginPlay() override;
};



