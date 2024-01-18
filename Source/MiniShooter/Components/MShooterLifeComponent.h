// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>

#include "MShooterLifeComponent.generated.h"

/**
* Component that adds Health and it's relevant functionality to an Actor
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINISHOOTER_API UMShooterLifeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMShooterLifeComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	* Setter for Health
 	* @param NewHealth Value for current Health
	*/
	void SetHealth(float NewHealth);

	/**
	* Getter for Health
 	*
  	* @return Value of current Health
	*/
	float GetHealth() { return Health; };

	/**
	* Getter for Max Health
 	*
  	* @return Value of the Max Health
	*/
	float GetMaxHealth() { return MaxHealth; };

protected:

	/**
	* MaxHealth that can be set from Editor
	*/
	UPROPERTY(EditAnywhere, Category = "Life Config")
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
};
