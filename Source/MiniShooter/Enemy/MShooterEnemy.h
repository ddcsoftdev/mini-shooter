// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

UCLASS()
class MINISHOOTER_API AMShooterEnemy : public AActor, public IMShooterEnemyBasics
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMShooterEnemy();

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
	*/
	UFUNCTION()
		void HandlePlayerWithinZone(bool bIsInsideZone);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	* Inflicts Damage
	*/
	UFUNCTION()
	virtual void TakeDamageAmount(float Amount) override;

	/**
	* Register belonging Patrol Zone
	*/
	UFUNCTION()
		AActor* GetActivePatrolZone() { return ActivePatrolZone; }

	/**
	* Register belonging Patrol Zone
	*/
	UFUNCTION()
		void RegisterPatrolZone(AActor* PatrolZone);

	/**
	* Unregister belonging Patrol Zone
	*/
	UFUNCTION()
		void UnRegisterPatrolZone(AActor* PatrolZone);

	/**
	* Updates TargetLocation for AI
	*/
	UFUNCTION()
		void UpdateAITargetLocation(AActor* PatrolPoint);

	/**
	* Toggels on and off the AI
	*/
	UFUNCTION()
		void SetAIBehaviour(bool bActivate);

};
