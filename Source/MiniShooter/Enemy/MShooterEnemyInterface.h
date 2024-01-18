// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "MShooterEnemyInterface.generated.h"

/**
* Interface created to hold basic Enemy template also applied to Shooting Targets
* Most functionality is specialized at the moment but add here anything relevant for future expansions of Enemies
*/
UINTERFACE(Blueprintable)
class MINISHOOTER_API UMShooterEnemyBasics : public UInterface
{
	GENERATED_UINTERFACE_BODY()
	
};

class MINISHOOTER_API IMShooterEnemyBasics
{
	GENERATED_IINTERFACE_BODY()

public:
	/**
	* Amount of Damage done to Actor
 	*
  	* @param Amount Value that is subtracted from Actor health
	*/
	virtual void TakeDamageAmount(float Amount);

protected:
	/**
	* Updates the HelthBar Widget
 	*
  	* @param MaxHealth Value that represents the max health of the Actor
   	* @param CurrentHealth Value that represents the current health of the Actor
	*/
	UFUNCTION()
		virtual void UpdateHealthBarWidget(float MaxHealth, float CurrentHealth);

	/**
	* Sets the Health Widget Rotation
	*/
	UFUNCTION()
		virtual void SetHealthWidgetRotation();
};
