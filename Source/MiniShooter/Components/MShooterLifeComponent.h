// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MShooterLifeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINISHOOTER_API UMShooterLifeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMShooterLifeComponent();

protected:

	/**
	* MaxHealth that can be set from Editor
	*/
	UPROPERTY(EditAnywhere, Category = "Life Settings")
	float MaxHealth{ 100.f };

	/**
	* Main health metter for component
	*/
	float Health{ 0.f };

	/**
	* Kills component owner
	*/
	void KillOwner();

	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	* Setter for Health
	*/
	void SetHealth(float NewHealth);

	/**
	* Getter for Health
	*/
	float GetHealth() { return Health; };

	/**
	* Getter for Max Health
	*/
	float GetMaxHealth() { return MaxHealth; };

		
};
