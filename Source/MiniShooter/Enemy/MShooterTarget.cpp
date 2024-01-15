// Fill out your copyright notice in the Description page of Project Settings.


#include "MShooterTarget.h"

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
AMShooterTarget::AMShooterTarget()
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
void AMShooterTarget::BeginPlay()
{
	Super::BeginPlay();

	//TODO: Remove this. Fixing Offset and making sure it always spawns correctly
	BoxCollision->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	HealthWidget->SetRelativeLocation(FVector(0.f, 0.f, 150.f));

	//Set Location for Inital Move Animation
	StartLocation = GetActorLocation();
	EndLocation = StartLocation + FVector(0.f, 0.f, MovementDistanceAmount);

}

// Called every frame
void AMShooterTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetHealthWidgetRotation();

	PlayTargetAnimation();
}

void AMShooterTarget::SetHealthWidgetRotation()
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

void AMShooterTarget::UpdateHealthBarWidget(float MaxHealth, float CurrentHealth)
{
	if (UMShooterHealthBarWidget* HealthBar = Cast<UMShooterHealthBarWidget>(HealthWidget->GetWidget()))
	{
		HealthBar->SetHealthBar(MaxHealth, CurrentHealth);
	}
}

void AMShooterTarget::TakeDamageAmount(float Amount)
{
	if (!LifeComponent)
	{
		return;
	}
	Amount = -Amount;
	LifeComponent->SetHealth(Amount);
	UpdateHealthBarWidget(LifeComponent->GetMaxHealth(), LifeComponent->GetHealth());
}

void AMShooterTarget::PlayTargetAnimation()
{
	const float SizeOffset = StaticMeshComponent->GetStaticMesh()->GetBounds().BoxExtent.Size();
	const float FloorLocation = 130.f;

	FVector MoveToLocation = GetActorLocation() + (GetActorUpVector() * TargetSpeed);

	//Check that target wont clip with floor (0.f Axis Z)
	//Using offset to account for Target size, since it counts from center of mesh
	if (MoveToLocation.Z - SizeOffset > FloorLocation)
	{
		SetActorLocation(MoveToLocation, true);
	}
	else
	{
		//Override movement and start over to avoid clipping
		MoveToLocation = EndLocation;
	}

	//Target has arrived to Location and change direction
	if (MoveToLocation.Equals(EndLocation, 10.f))
	{
		if (StartLocation.Z < MoveToLocation.Z)
		{
			StartLocation = EndLocation;
			EndLocation = EndLocation - MovementDistanceAmount;
			TargetSpeed = TargetSpeed * -1.f;
		}
		else
		{
			StartLocation = EndLocation;
			EndLocation = EndLocation + MovementDistanceAmount;
			TargetSpeed = TargetSpeed * -1.f;
		}
	}
}

