// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MShooterEnemy.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

class AMShooterAIController;
class UMShooterLifeComponent;

UCLASS()
class MINISHOOTER_API AMShooterEnemy : public AActor
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
	* Visible Mesh for Enemy
	*/
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;

	/**
	* Decouple Collision from Mesh for flexibility
	*/
	UPROPERTY(EditAnywhere)
		UBoxComponent* BoxCollision;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
