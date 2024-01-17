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

	//Spawning Scene Components giving weird offset when importing in Scene
	//Suggestion: consider adding to design creating this from BP and use C++ just to link them
	BoxCollision->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	HealthWidget->SetRelativeLocation(FVector(0.f, 0.f, 150.f));

	//Kickstarting animation first "MoveTo" location
	StartLocation = GetActorLocation();
	EndLocation = StartLocation + FVector(0.f, 0.f, AnimationDistanceAmount);
}

// Called every frame
void AMShooterTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Modifying Health Widget Rotation per frame so it faces Player
	SetHealthWidgetRotation();
	//Modifying Actor location per frame. Simple movement so no DeltaTime needed for Lerp or Interp
	PlayTargetAnimation();
}

void AMShooterTarget::SetHealthWidgetRotation()
{
	if (ensureMsgf(HealthWidget && GetWorld(), TEXT("%s couldn't load %s or %s at Runtime"), *GetClass()->GetName(), *HealthWidget->GetClass()->GetName(), *GetWorld()->GetClass()->GetName()))
	{
		if (ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		{
			//Setting Health Widget Rotation so it always faces Player
			HealthWidget->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation()));
		}
	}
}

void AMShooterTarget::UpdateHealthBarWidget(float MaxHealth, float CurrentHealth)
{
	if (ensureMsgf(HealthWidget, TEXT("%s couldn't load %s at Runtime"), *GetClass()->GetName(), *HealthWidget->GetClass()->GetName()))
	{
		if (UMShooterHealthBarWidget* HealthBar = Cast<UMShooterHealthBarWidget>(HealthWidget->GetWidget()))
		{
			//Setting Health Bar Widget new value
			HealthBar->SetHealthBar(MaxHealth, CurrentHealth);
		}
	}
}

void AMShooterTarget::TakeDamageAmount(float Amount)
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

void AMShooterTarget::PlayTargetAnimation()
{
	float SizeOffset = 0.f;
	float FloorLocation = 0.f;
	if (ensureMsgf(StaticMeshComponent, TEXT("%s couldn't load %s at Runtime"), *GetClass()->GetName(), *StaticMeshComponent->GetClass()->GetName()))
	{
		//Get where the floor is based on the Actor's Mesh. This is used to avoid Actor going through floor
		SizeOffset = StaticMeshComponent->GetStaticMesh()->GetBounds().BoxExtent.Size() / 2;
		FloorLocation = 130.f;
	}
	
	//Calculate New Location for Actor to Move To
	FVector MoveToLocation = GetActorLocation() + (GetActorUpVector() * AnimationSpeed);

	//Check possiblity of Actor going through floor and avoid if it's predicted
	if (MoveToLocation.Z - SizeOffset > FloorLocation)
	{
		SetActorLocation(MoveToLocation, true);
	}
	else
	{
		//Override Location to immediatly reach endpoint assuming it would go through floor and clip
		MoveToLocation = EndLocation;
	}

	//Check Actor has arrived to desired location, and prepare to reverse motion backwards
	if (MoveToLocation.Equals(EndLocation, 10.f))
	{
		//Checking automatically if Actor is supposed to go up or down based on where is EndLocation relative to StartLocation
		if (StartLocation.Z < MoveToLocation.Z)
		{
			StartLocation = EndLocation;
			EndLocation = EndLocation - AnimationDistanceAmount;
		}
		else
		{
			StartLocation = EndLocation;
			EndLocation = EndLocation + AnimationDistanceAmount;
		}
		//Use Animation Speed as the driver for direction, simply reversing for next iterations of this method
		AnimationSpeed = -AnimationSpeed;
	}
}

