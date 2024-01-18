// Fill out your copyright notice in the Description page of Project Settings.


#include "MShooterKillCountWidget.h"

void UMShooterKillCountWidget::SetCount(int32 Count)
{
	EnemyCount = Count;
	UpdateTextBlock();
}

void UMShooterKillCountWidget::UpdateTextBlock()
{
	FString FormattedString = FString::Printf(TEXT("Enemies Killed: %d"), EnemyCount);
	EnemyCountMessage->SetText(FText::FromString(FormattedString));
}