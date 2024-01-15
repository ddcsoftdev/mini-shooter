// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MShooterProjectile.generated.h"

UCLASS()
class MINISHOOTER_API AMShooterProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMShooterProjectile();

	/**
	* Override projectile speed
	*/
	void OverrideProjectileSpeed(float OverrideProjectileSpeed);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	* Speed for the projectile
	*/
	UPROPERTY(EditAnywhere)
		float ProjectileSpeed{ 10.f };

		/**
		* Moves projectile forward in a straigh line
		*/

	UFUNCTION()
		void MoveProjectile(float DeltaTime);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
