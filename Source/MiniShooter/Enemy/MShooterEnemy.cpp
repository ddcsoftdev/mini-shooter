// Fill out your copyright notice in the Description page of Project Settings.


#include "MShooterEnemy.h"

#include <Components/StaticMeshComponent.h>
#include <Components/BoxComponent.h>
#include <Components/WidgetComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>
#include <GameFramework/Character.h>

#include "../Components/MShooterLifeComponent.h"
#include "../Components/MShooterShootingComponent.h"
#include "../UI/MShooterHealthBarWidget.h"

// Sets default values
AMShooterEnemy::AMShooterEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create Life Component
	LifeComponent = CreateDefaultSubobject<UMShooterLifeComponent>(FName(TEXT("LifeComponent")));
	//Create Shooting Component
	ShootingComponent = CreateDefaultSubobject<UMShooterShootingComponent>(FName(TEXT("ShootingComponent")));
	//Create Static Mesh Component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("StaticMesh")));
	//Create Box Collision Box
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(FName(TEXT("Collision")));
	//Create Widget Component
	HealthWidget = CreateDefaultSubobject<UWidgetComponent>(FName(TEXT("HealthBar")));

}

// Called when the game starts or when spawned
void AMShooterEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	//TODO: Remove this. Fixing Offset and making sure it always spawns correctly
	BoxCollision->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	HealthWidget->SetRelativeLocation(FVector(0.f, 0.f, 150.f));

}

// Called every frame
void AMShooterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetHealthWidgetRotation();
}

void AMShooterEnemy::SetHealthWidgetRotation()
{
	if (!HealthWidget)
	{
		return;
	}

	if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		HealthWidget->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation()));
	}
}

void AMShooterEnemy::UpdateHealthBarWidget(float MaxHealth, float CurrentHealth)
{
	if (UMShooterHealthBarWidget* HealthBar = Cast<UMShooterHealthBarWidget>(HealthWidget->GetWidget()))
	{
		HealthBar->SetHealthBar(MaxHealth, CurrentHealth);
	}
}

void AMShooterEnemy::TakeDamage(float Amount)
{
	if (!LifeComponent)
	{
		return;
	}
	Amount = -Amount;
	LifeComponent->SetHealth(Amount);
	UpdateHealthBarWidget(LifeComponent->GetMaxHealth(), LifeComponent->GetHealth());
}
