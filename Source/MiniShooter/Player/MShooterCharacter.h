// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "../MiniShooterCharacter.h"
#include "MShooterCharacter.generated.h"


class UMShooterShootingComponent;

UCLASS(config=Game)
class AMShooterCharacter : public AMiniShooterCharacter
{
	GENERATED_BODY()

public:

	AMShooterCharacter();

protected:

	/**
	* Shooting Component
	*/
	UPROPERTY(EditAnywhere)
	UMShooterShootingComponent* ShootingComponent;
};

