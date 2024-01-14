// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MShooterHealthBarWidget.generated.h"

class UProgressBar;

UCLASS()
class MINISHOOTER_API UMShooterHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UProgressBar* HealthBar;
public:

	UFUNCTION()
		void SetHealthBar(float MaxHealth, float CurrentHealth);
	
};
