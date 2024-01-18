// Fill out your copyright notice in the Description page of Project Settings.


#include "MShooterHealthBarWidget.h"

#include <Components/ProgressBar.h>

void UMShooterHealthBarWidget::SetHealthBar(float MaxHealth, float CurrentHealth)
{
	if (ensureMsgf(IsValid(HealthBar), TEXT("%s faced error at Runtime"), *GetClass()->GetName()))
	{
		float Percentage = (CurrentHealth * 1.f) / MaxHealth;
		HealthBar->SetPercent(Percentage);
	}
}