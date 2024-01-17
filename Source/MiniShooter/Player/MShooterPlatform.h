// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MShooterPlatform.generated.h"

UCLASS()
class MINISHOOTER_API AMShooterPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMShooterPlatform();

protected:

	/**
	* New Shooting Speed for player upon entering platform
	*/
	UPROPERTY(EditAnywhere, Category = "Platform Config")
		float NewShootingSpeed{ 10.f };

	/**
	* Old Shooting Speed to be able to restore
	*/
	float BackupShootingSpeed{ 0.f };

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION()
		virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
