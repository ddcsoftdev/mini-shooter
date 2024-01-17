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
	
	//Spawning Scene Components giving weird offset when importing in Scene
	//Suggestion: consider adding to design creating this from BP and use C++ just to link them
	BoxCollision->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	HealthWidget->SetRelativeLocation(FVector(0.f, 0.f, 150.f));
}

// Called every frame
void AMShooterEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Modifying Health Widget Rotation per frame so it faces Player
	SetHealthWidgetRotation();
}

void AMShooterEnemy::SetHealthWidgetRotation()
{
	if (ensureMsgf(HealthWidget && GetWorld(), TEXT("%s couldn't load %s or %s at Runtime"), *GetClass()->GetName(), *HealthWidget->GetClass()->GetName(), *GetWorld()->GetClass()->GetName()))
	{
		if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		{
			//Rotating HealthWidget once per frame so it always faces Player
			HealthWidget->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation()));
		}
	}
}

void AMShooterEnemy::UpdateHealthBarWidget(float MaxHealth, float CurrentHealth)
{
	if (ensureMsgf(HealthWidget, TEXT("%s couldn't load %s at Runtime"), *GetClass()->GetName(), *HealthWidget->GetClass()->GetName()))
	{
		if (UMShooterHealthBarWidget* HealthBar = Cast<UMShooterHealthBarWidget>(HealthWidget->GetWidget()))
		{
			//Set value of Health Bar
			HealthBar->SetHealthBar(MaxHealth, CurrentHealth);
		}
	}
}

void AMShooterEnemy::TakeDamageAmount(float Amount)
{
	if (ensureMsgf(LifeComponent, TEXT("%s couldn't load %s at Runtime"), *GetClass()->GetName(), *LifeComponent->GetClass()->GetName()))
	{
		//Invert Damage Amount to negative to subtract
		Amount = -Amount;
		//Setting Health in Life Component
		LifeComponent->SetHealth(Amount);
		//Updating Widget
		UpdateHealthBarWidget(LifeComponent->GetMaxHealth(), LifeComponent->GetHealth());
	}
}

void AMShooterEnemy::RegisterPatrolZone(AActor* PatrolZone)
{
	if (IsValid(PatrolZone))
	{
		if (AMShooterPatrolZone* CastedZone = Cast<AMShooterPatrolZone>(PatrolZone))
		{
			//If Enemy is within a Patrol Zone, register it in variable for convenience
			ActivePatrolZone = PatrolZone;
			//Bind Delegate to know when Player is within this Patrol Zone
			CastedZone->PlayerIsInsideZoneDelegate.AddUObject(this, &AMShooterEnemy::HandlePlayerWithinZone);
		}
	}
}

void AMShooterEnemy::UnRegisterPatrolZone(AActor* PatrolZone)
{
	if (IsValid(PatrolZone))
	{
		if (AMShooterPatrolZone* CastedZone = Cast<AMShooterPatrolZone>(PatrolZone))
		{
			if (ActivePatrolZone == PatrolZone)
			{
				//When Actor leaves Patrol Zone make sure the reference in variable is wiped
				ActivePatrolZone = nullptr;
			}
		}
	}
}

void AMShooterEnemy::HandlePlayerWithinZone(bool bIsInsideZone)
{
	if (ensureMsgf(AIComponent, TEXT("%s couldn't load %s at Runtime"), *GetClass()->GetName(), *AIComponent->GetClass()->GetName()))
	{
		//Tell AI Component that Player has entered / left the Patrol Zone this Actor belongs to
		if (bIsInsideZone)
		{
			AIComponent->StartPlayerChase();
		}
		else
		{
			AIComponent->StopPlayerChase();
		}
	}
}

void AMShooterEnemy::UpdateAITargetLocation(AActor* PatrolPoint)
{
	if (ensureMsgf(AIComponent, TEXT("%s couldn't load %s at Runtime"), *GetClass()->GetName(), *AIComponent->GetClass()->GetName()))
	{
		if (AMShooterPatrolZone* CastedZone = Cast<AMShooterPatrolZone>(ActivePatrolZone))
		{
			//Tries to return the old Patrol Point this Actor was using as Location to the Patrol Zone pool
			CastedZone->TryToReturnPatrolPoint(ActivePatrolPoint);
		}
		//Sets the new Patrol Point sent by the Patrol Zone
		ActivePatrolPoint = PatrolPoint;
		//Sends the Patrol Point location to the AI Component to set course towrads the new location
		AIComponent->SetTargetLocation(PatrolPoint->GetActorLocation());
	}
}

void AMShooterEnemy::SetAIBehaviour(bool bActivate)
{
	if (ensureMsgf(AIComponent, TEXT("%s couldn't load %s at Runtime"), *GetClass()->GetName(), *AIComponent->GetClass()->GetName()))
	{
		//This Activates or Deactivates this Actor's AI Behaviour
		AIComponent->SetAIBehaviour(bActivate);
	}
}
