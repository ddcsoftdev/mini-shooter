// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "MShooterPatrolZone.generated.h"

class AMShooterPatrolPoint;

DECLARE_MULTICAST_DELEGATE_TwoParams(FRegisterPatrolPointUsage, class AActor*, bool bIsBeingUsed);
DECLARE_MULTICAST_DELEGATE_OneParam(FSendPatrolPoint, class AActor*);
DECLARE_MULTICAST_DELEGATE_OneParam(FPlayerIsInsideZone, bool bIsInside);

/**
* Actor that represents a Patrol Zone where Enemies can patrol.
* It uses Patrol Points to define valid locations to patrol within it's boundries
* It alerts Enemies of a Player entering and leaving the Patrol Zone
*/
UCLASS()
class MINISHOOTER_API AMShooterPatrolZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMShooterPatrolZone();

	/**
	* Delegate for registering in use Patrol Points
	*/
		FRegisterPatrolPointUsage RegisterPatrolPointInUserDelegate;

	/**
	* Sends available patrol point
	* Takes the Enemy as argument to know who to send it to
	*/
		FSendPatrolPoint SendPatrolPointDelegate;

	/**
	* Sends available patrol point
	* Takes the Enemy as argument to know who to send it to
	*/
		FPlayerIsInsideZone PlayerIsInsideZoneDelegate;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	* Wrapper function Try to return used Patrol Point from other classes
	*/
	UFUNCTION()
		void TryToReturnPatrolPoint(AActor* PatrolPoint);

	/**
	* Initialize Component
	*/
	UFUNCTION()
		void Initialize();

	/**
	* Ensure that Enemy Should keep chaisng as a failsafe
 	*
  	* @param Enemy reference to the Enemy sending the request
	*/
	UFUNCTION()
		void CheckIfPlayerStillInZone(AActor* Enemy);

protected:

	/**
	* Available Patrol Points for this Zone
	*/
	UPROPERTY()
	TArray<AActor*> AvailablePatrolPoints;

	/**
	* Enemies within Patrol Zone
	*/
	UPROPERTY()
		TArray<AActor*> ActiveEnemies;

	/**
	* Control variable for Player within Zone
	*/
	UPROPERTY()
		bool bIsPlayerInZone{ false };

	/**
	* Get Available Patrol Points
 	*
  	* @return Available Patrol Point as an Actor or nullptr
	*/
	UFUNCTION()
		AActor* GetAvailablePatrolPoint();

	/**
	* Register Available Patrol Points when not detected in Array
 	*
  	* @param PatrolPoint Actor to register within this Patrol Zone
	*/
	UFUNCTION()
		void RegisterPatrolPoint(AActor* PatrolPoint);

	/**
	* Register Enemies upon entering patrol zone
 	*
  	* @param Enemy Actor to register as Enemy within this Patrol Zone
	*/
	UFUNCTION()
		void RegisterEnemy(AActor* Enemy);

	/**
	* UnRegister Available Patrol Points
 	*
  	* @param PatrolPoint Actor to delete from this Patrol Zone
	*/
	UFUNCTION()
		void UnRegisterPatrolPoint(AActor* PatrolPoint);

	/**
	* UnRegister Enemies upon entering patrol zone
 	*
  	* @param Enemy Actor to delete from this Patrol Zone
	*/
	UFUNCTION()
		void UnRegisterEnemy(AActor* Enemy);

	/**
	* Notify all enemies player has entered zone
 	*
  	* @param Enemy Actor that is sending the request
	*/
	UFUNCTION()
		void NotifyPlayerEnteredZone(AActor* Enemy);

	/**
	* Notify all enemies player has left zone
 	*
  	* @paramo Enemy Actor that is sending the request
	*/
	UFUNCTION()
		void NotifyPlayerLeftZone(AActor* Enemy);

	/***
	* Get Random Point within Patrol Zone
 	*
  	* @param EnemyToSend Actor that it sends Patrol Point to
	*/
	UFUNCTION()
	void SendPatrolPoint(AActor* EnemyToSend);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION()
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

};
