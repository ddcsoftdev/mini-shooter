// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "MShooterProjectile.generated.h"

class UStaticMeshComponent;

/**
* Projectile class that is to be launched by Player and Enemies.
* It also handles collision and the resulting effects such as health modification.
*/
UCLASS()
class MINISHOOTER_API AMShooterProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMShooterProjectile();

	/**
	* Override projectile speed. This is an option giving to the shooting user and handled by MShooterShootingCcomponent.
	*
 	* @param OverrideProjectileSpeed Value to change the speed of the projectile
 	*/
	UFUNCTION()
		void OverrideProjectileSpeed(float OverrideProjectileSpeed);

	/**
	* Set Registered Owner
 	*
  	* @param RegisterOwner Actor to register as the Owner of the projectile
	*/
	UFUNCTION()
		void RegisterProjectileOwner(AActor* RegisterOwner);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	/**
	* Visible Mesh for for projectile
	*/
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;

	/**
	* Speed for the projectile
	*/
	UPROPERTY(EditAnywhere, Category = "Projectile Config")
		float ProjectileSpeed{ 10.f };

	/**
	* Damage for the projectile
	*/
	UPROPERTY(EditAnywhere, Category = "Projectile Config")
		float ProjectileDamage{ 25.f };

	/**
	* Timer handle for Destroy Actor
	*/
	UPROPERTY()
		FTimerHandle DestroyTimerHandle;

	/**
	* Time To Destroy after Spawning. Used to avoid uncesseary unused projectiles stacking up in map
	*/
	UPROPERTY(EditAnywhere, Category = "Projectile Config")
		float TimeToDestroy{ 3.f };

	/**
	* Registered Owner of this Projectile
	*/
	UPROPERTY()
		AActor* RegisteredOwnerOfProjectile;

	/**
	* Moves projectile forward in a straight line
 	*
  	* @param DeltaTime Value to pass a valid DeltaTime
	*/
	UFUNCTION()
		void MoveProjectile(float DeltaTime);

	/**
	* Kickstarts AutoDestroy Timer so no map overflow
	*/
	UFUNCTION()
		void StartDestroyTimer();

	/**
	* Starts Destroy schedule
	*/
	UFUNCTION()
		void RequestProjectileDestroy();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
