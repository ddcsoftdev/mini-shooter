// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "MShooterEnemyInterface.h"

#include "MShooterTarget.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UWidgetComponent;

class AMShooterAIController;
class UMShooterLifeComponent;
class UMShooterShootingComponent;

/**
* Target class that simulates a Shooting Range Target
* It has a simple up and down animation
* Has same functionality as Enemy except for this class lacking AI
* Implements Enemy Interface
*/
UCLASS()
class MINISHOOTER_API AMShooterTarget : public AActor, public IMShooterEnemyBasics
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMShooterTarget();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	* Inflicts Damage
	* Inverts Amount value to negative to subtract
 	*
  	* @param Amount Value that is subtracted to the Actor's health
	*/
	UFUNCTION()
		virtual void TakeDamageAmount(float Amount) override;
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
	* Amount the target moves up and down
	*/
	UPROPERTY(EditAnywhere, Category = "Shooting Target Config")
		float AnimationDistanceAmount{ 150.f };

	/**
	* Animation Speed
	*/
	UPROPERTY(EditAnywhere, Category = "Shooting Target Config")
		float AnimationSpeed{ 3.f };

	/**
	* Start and End Location for Animation
	*/
	UPROPERTY()
		FVector StartLocation {FVector::ZeroVector};
	UPROPERTY()
		FVector EndLocation {FVector::ZeroVector};

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	* Internal update health bar widget
	* It uses Health and MaxHealth Attributes from class
 	*
  	* @param MaxHealth Value that represents max health of the Actor
   	* @param CurrentHealth Value that represent current health of the Actor
	*/
	UFUNCTION()
		virtual void UpdateHealthBarWidget(float MaxHealth, float CurrentHealth) override;

	/**
	* Rotate Healthbar Widget
	*/
	UFUNCTION()
		virtual void SetHealthWidgetRotation() override;
	
	/**
	* Move Target Up and Down
	*/
	UFUNCTION()
		void PlayTargetAnimation();
};
