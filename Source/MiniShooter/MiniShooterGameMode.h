// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MiniShooterGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FSendNearestEnemy, class AActor*);
DECLARE_MULTICAST_DELEGATE_OneParam(FNotifyEnemyDead, class AActor*);


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
	*/
	UFUNCTION()
	void RemoveDeadEnemyFromActiveEnemies(AActor* DeadEnemy);

	/**
	* Method that processes RequestEnemyDelegate 
	*/
	UFUNCTION()
	void ProcessRequestEnemyDelegate(AActor* RequestingActor);

	virtual void BeginPlay() override;


};



