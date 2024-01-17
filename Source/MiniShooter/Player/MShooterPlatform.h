// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "MShooterPlatform.generated.h"

/**
* Platform class that enables a Shooting Speed modification for the Player when standing over it.
* Shooting Speed is restored to normal when Player moves off the platform
*/
UCLASS()
class MINISHOOTER_API AMShooterPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMShooterPlatform();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:

	/**
	* New Shooting Speed for player upon entering platform
	*/
	UPROPERTY(EditAnywhere, Category = "Platform Config")
		float NewShootingSpeed{ 10.f };

	/**
	* Backup Shooting Speed to be able to restore to old value
	*/
	float BackupShootingSpeed{ 0.f };

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION()
		virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
};
