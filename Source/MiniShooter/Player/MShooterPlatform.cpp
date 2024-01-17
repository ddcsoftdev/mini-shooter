// Fill out your copyright notice in the Description page of Project Settings.


#include "MShooterPlatform.h"

#include "MShooterCharacter.h"

// Sets default values
AMShooterPlatform::AMShooterPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMShooterPlatform::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AMShooterPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMShooterPlatform::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//If Player enters platform, then store current Shooting Speed value and apply new one
	if (OtherActor->GetClass()->GetSuperClass() == AMShooterCharacter::StaticClass())
	{
		if (AMShooterCharacter* Player = Cast<AMShooterCharacter>(OtherActor))
		{
			BackupShootingSpeed = Player->GetShootingSpeed();
			Player->SetShootingSpeed(NewShootingSpeed);
		}
	}
}

void AMShooterPlatform::NotifyActorEndOverlap(AActor* OtherActor)
{
	//If Player leaves platform, then restore Shooting Speed to the value before entering
	if (OtherActor->GetClass()->GetSuperClass() == AMShooterCharacter::StaticClass())
	{
		if (AMShooterCharacter* Player = Cast<AMShooterCharacter>(OtherActor))
		{
			Player->SetShootingSpeed(BackupShootingSpeed);
		}
	}
}