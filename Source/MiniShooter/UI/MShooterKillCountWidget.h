// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Blueprint/UserWidget.h>
#include <Components/TextBlock.h>

#include "MShooterKillCountWidget.generated.h"

/**
 * Widget that show Player how many Enemies he has killed
 */
UCLASS()
class MINISHOOTER_API UMShooterKillCountWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/**
	* Sets Enemy Count
	* 
	* @param Count Value that updates EnemyCount
	*/
	void SetCount(int32 Count);

private:
	/**
	* Tracks the amount of dead enemies and displays it
	*/
	UPROPERTY()
		int32 EnemyCount { 0 };

	/**
	* TextBlock that displays EnemyCount along with a message
	*/
	UPROPERTY(EditAnywhere, meta = (BindWidget))
		UTextBlock* EnemyCountMessage;

	/**
	* Method that updates the text block in the Widget
	*/
	UFUNCTION()
		void UpdateTextBlock();
};
