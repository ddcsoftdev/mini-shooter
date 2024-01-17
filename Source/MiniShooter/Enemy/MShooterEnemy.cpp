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
#include "../AI/MShooterAIComponent.h"
#include "../AI/MShooterPatrolZone.h"

// Sets default values
AMShooterEnemy::AMShooterEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create Life Component
	LifeComponent = CreateDefaultSubobject<UMShooterLifeComponent>(FName(TEXT("LifeComponent")));
	//Create Shooting Component
	ShootingComponent = CreateDefaultSubobject<UMShooterShootingComponent>(FName(TEXT("ShootingComponent")));
	//Create Shooting Component
	AIComponent = CreateDefaultSubobject<UMShooterAIComponent>(FName(TEXT("AIComponent")));
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

void AMShooterEnemy::TakeDamageAmount(float Amount)
{
	if (!LifeComponent)
	{
		return;
	}
	Amount = -Amount;
	LifeComponent->SetHealth(Amount);
	UpdateHealthBarWidget(LifeComponent->GetMaxHealth(), LifeComponent->GetHealth());
}

void AMShooterEnemy::RegisterPatrolZone(AActor* PatrolZone)
{
	//Quick way to tell if class is correct
	if (AMShooterPatrolZone* CastedZone = Cast<AMShooterPatrolZone>(PatrolZone))
	{
		ActivePatrolZone = PatrolZone;
		CastedZone->PlayerIsInsideZoneDelegate.AddUObject(this, &AMShooterEnemy::HandlePlayerWithinZone);
	}
}

void AMShooterEnemy::UnRegisterPatrolZone(AActor* PatrolZone)
{
	if (AMShooterPatrolZone* CastedZone = Cast<AMShooterPatrolZone>(PatrolZone))
	{
		if (ActivePatrolZone == PatrolZone)
		{
			ActivePatrolZone = nullptr;
		}
	}
}

void AMShooterEnemy::HandlePlayerWithinZone(bool bIsInsideZone)
{
	if (!AIComponent)
	{
		return;
	}

	if (bIsInsideZone)
	{
		AIComponent->StartPlayerChase();
	}
	else
	{
		AIComponent->StopPlayerChase();
	}
}

void AMShooterEnemy::UpdateAITargetLocation(AActor* PatrolPoint)
{
	if (!AIComponent)
	{
		return;
	}
	if (AMShooterPatrolZone* CastedZone = Cast<AMShooterPatrolZone>(ActivePatrolZone))
	{
		CastedZone->TryToReturnPatrolPoint(ActivePatrolPoint);
	}

	ActivePatrolPoint = PatrolPoint;
	AIComponent->SetTargetLocation(PatrolPoint->GetActorLocation());
}

void AMShooterEnemy::SetAIBehaviour(bool bActivate)
{
	if (!AIComponent)
	{
		return;
	}

	AIComponent->SetAIBehaviour(bActivate);
}
