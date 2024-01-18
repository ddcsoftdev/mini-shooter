// Fill out your copyright notice in the Description page of Project Settings.


#include "MShooterHUD.h"

#include "MShooterKillCountWidget.h"
AMShooterHUD::AMShooterHUD()
{
	//Get BP Class for the Projectile in order to be able to Spawn it when Shooting
	ConstructorHelpers::FObjectFinder<UBlueprint> WidgetBPFinder(TEXT("Blueprint'/Game/MiniShooter/Widgets/KillCount_WB.KillCount_WB'"));
	if (WidgetBPFinder.Object)
	{
		KillCountWidgetClass = (UClass*)WidgetBPFinder.Object->GeneratedClass;
	}
}

void AMShooterHUD::BeginPlay()
{
	Super::BeginPlay();

	//Bind delegate
	UpdateKillCountDelegate.AddUObject(this, &AMShooterHUD::UpdateKillCout);

	//Add Widget to Viewport
	if (ensureMsgf(KillCountWidgetClass && GetWorld(), TEXT("%s couldn't load %s at Runtime"), *GetClass()->GetName(), *KillCountWidgetClass->GetClass()->GetName(), *GetWorld()->GetClass()->GetName()))
	{
		if (!IsValid(KillCountWidget))
		{
			KillCountWidget = CreateWidget<UMShooterKillCountWidget>(GetWorld(), KillCountWidgetClass);
			if (IsValid(KillCountWidget))
			{
				//Init message and count to 0
				KillCountWidget->SetCount(0);
				//Add to viewport
				KillCountWidget->AddToViewport();
				//Set visibility
				KillCountWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void AMShooterHUD::UpdateKillCout(int32 Count)
{
	if (IsValid(KillCountWidget))
	{
		KillCountWidget->SetCount(Count);
	}
}