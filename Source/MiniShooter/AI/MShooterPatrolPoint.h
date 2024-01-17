// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MShooterPatrolPoint.generated.h"

/**
* Class used to indicate Location for Enemies within a Patrol Zone
* It has no furthuer functionality at the moment
*/
UCLASS()
class MINISHOOTER_API AMShooterPatrolPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMShooterPatrolPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
