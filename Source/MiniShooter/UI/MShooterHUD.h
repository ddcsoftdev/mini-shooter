// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/HUD.h>

#include "MShooterHUD.generated.h"

class UMShooterKillCountWidget;

DECLARE_MULTICAST_DELEGATE_OneParam(FUpdateKillCount, int32);

/**
 * Custom HUD that holds all UI widgets for player
 */
UCLASS()
class MINISHOOTER_API AMShooterHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMShooterHUD();

	/**
	* Update kill count delegate
	*/
	FUpdateKillCount UpdateKillCountDelegate;

protected:
	/**
	* Widget Class that will load the KillCount Widget
	*/
	UPROPERTY()
		TSubclassOf<UMShooterKillCountWidget> KillCountWidgetClass;

	/**
	* Widget for the Kill Count that is loaded at BeginPlay if not set in editor
	*/
	UPROPERTY(EditDefaultsOnly, Category = "HUD Config")
		UMShooterKillCountWidget* KillCountWidget;

	virtual void BeginPlay() override;

	/**
	* Method that updates the kill count int
	* 
	* @param Count Amount of killed enemies
	*/
	UFUNCTION(Blueprintcallable)
		void UpdateKillCout(int32 Count);
};
