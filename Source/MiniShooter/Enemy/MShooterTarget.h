// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MShooterEnemyInterface.h"
#include "MShooterTarget.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UWidgetComponent;

class AMShooterAIController;
class UMShooterLifeComponent;
class UMShooterShootingComponent;

UCLASS()
class MINISHOOTER_API AMShooterTarget : public AActor, public IMShooterEnemyBasics
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMShooterTarget();

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
	UPROPERTY(EditAnywhere, Category = "Target Movement")
		float MovementDistanceAmount{ 150.f };

	/**
	* Target Speed
	*/
	UPROPERTY(EditAnywhere, Category = "Target Movement")
		float TargetSpeed{ 3.f };

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
	*/
	UFUNCTION()
		virtual void UpdateHealthBarWidget(float MaxHealth, float CurrentHealth) override;

	/**
	* Rotate Healthbar
	*/
	UFUNCTION()
		virtual void SetHealthWidgetRotation() override;
	
	/**
	* Move Target Up and Down
	*/
	UFUNCTION()
	void PlayTargetAnimation();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	* Inflicts Damage
	*/
	UFUNCTION()
		virtual void TakeDamageAmount(float Amount) override;

};
