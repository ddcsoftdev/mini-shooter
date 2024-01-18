// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "MShooterEnemyInterface.h"

#include "MShooterEnemy.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UWidgetComponent;

class AMShooterAIController;
class UMShooterLifeComponent;
class UMShooterShootingComponent;
class UMShooterAIComponent;
class UMShooterPatrolZone;

/**
* Main Enemy class with all the relevant functionality
* Relevant AI, Shooting and Life functionality is mostly done in correspondant Components better modularity
* Implements some methods from Enemy Interface
*/
UCLASS()
class MINISHOOTER_API AMShooterEnemy : public AActor, public IMShooterEnemyBasics
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMShooterEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	* Inflicts Damage
 	*
  	* @param Amount Value that subtracts to current Health
	*/
	UFUNCTION()
		virtual void TakeDamageAmount(float Amount) override;

	/**
	* Register belonging Patrol Zone
 	*
  	* @return Actor that points to the Patrol Zone or nullptr
	*/
	UFUNCTION()
		AActor* GetActivePatrolZone() { return ActivePatrolZone; }

	/**
	* Register belonging Patrol Zone
 	*
  	* @param PatrolZone Actor that is set as active Patrol Zone
	*/
	UFUNCTION()
		void RegisterPatrolZone(AActor* PatrolZone);

	/**
	* Unregister belonging Patrol Zone
 	*
  	* @param PatrolZone Actor which is unreferenced as the active Patrol Zone
	*/
	UFUNCTION()
		void UnRegisterPatrolZone(AActor* PatrolZone);

	/**
	* Updates TargetLocation for AI
 	*
  	* @param PatrolPoint Actor that updates the AI Component TargetLocation
	*/
	UFUNCTION()
		void UpdateAITargetLocation(AActor* PatrolPoint);

	/**
	* Toggels on and off the AI
 	*
  	* @param bActivate Value that activates or deactivates the AI Component
	*/
	UFUNCTION()
		void SetAIBehaviour(bool bActivate);

protected:

	/**
	* Life Component that handles enemy life
	*/
	UPROPERTY(EditAnywhere)
		UMShooterLifeComponent* LifeComponent;

	/**
	* Shooting Component
	*/
	UPROPERTY(EditAnywhere)
		UMShooterShootingComponent* ShootingComponent;

	/**
	* AI Component
	*/
	UPROPERTY(EditAnywhere)
		UMShooterAIComponent* AIComponent;

	/**
	* Visible Mesh for Enemy
	*/
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;

	/**
	* Decouple Collision from Mesh for flexibility
	*/
	UPROPERTY(EditAnywhere)
		UBoxComponent* BoxCollision;

	/**
	* Health Widget
	*/
	UPROPERTY(EditAnywhere)
		UWidgetComponent* HealthWidget;

	/**
	* Belonging Patrol Zone if Any
	*/
	UPROPERTY()
		AActor* ActivePatrolZone;

	/**
	* Belonging Patrol Zone if Any
	*/
	UPROPERTY()
		AActor* ActivePatrolPoint;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	* Internal update health bar widget
	* It uses Health and MaxHealth Attributes from class
	* 
 	* @param MaxHealth Value that represents max health of the Actor
  	* @param CurrentHealth Value that represents the current health of the Actor
	*/
	UFUNCTION()
		virtual void UpdateHealthBarWidget(float MaxHealth, float CurrentHealth) override;

	/**
	* Rotate Healthbar
	*/
	UFUNCTION()
		virtual void SetHealthWidgetRotation() override;

	/**
	* Comunicates AI Component about player within zone
 	*
  	* @param bIsInsideZone Value that represents if Player is within ActivePatrolZone
	*/
	UFUNCTION()
		void HandlePlayerWithinZone(bool bIsInsideZone);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
