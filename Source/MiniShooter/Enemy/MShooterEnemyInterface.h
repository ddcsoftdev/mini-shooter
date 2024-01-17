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
	virtual void TakeDamageAmount(float Amount);

protected:
	UFUNCTION()
	virtual void UpdateHealthBarWidget(float MaxHealth, float CurrentHealth);

	UFUNCTION()
	virtual void SetHealthWidgetRotation();
};
