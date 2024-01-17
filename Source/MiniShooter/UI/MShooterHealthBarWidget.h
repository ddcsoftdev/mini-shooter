// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>

#include "MShooterHealthBarWidget.generated.h"

class UProgressBar;

/**
* Widget that creates a World Spawnable Widget to show Enemies and Targets health.
*/
UCLASS()
class MINISHOOTER_API UMShooterHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	/**
	* Progress Bar for Health bar purposes
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UProgressBar* HealthBar;
public:

	/**
	* Setter to manipulate progress within progress bar
	*/
	UFUNCTION()
		void SetHealthBar(float MaxHealth, float CurrentHealth);
	
};
