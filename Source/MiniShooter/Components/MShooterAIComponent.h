// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <Components/ActorComponent.h>

#include "MShooterAIComponent.generated.h"

/**
* State Machine represented as an Enum for all possible AI States
*/
UENUM(BlueprintType)
enum class EStateMachine : uint8
{
	Idle	UMETA(DisplayName = "Idle"),
	Patrol	UMETA(DisplayName = "Patrol"),
	Chase	UMETA(DisplayName = "Chase"),
	Rest    UMETA(DisplayName = "Rest"),
	Attack  UMETA(DisplayName = "Attack")
};

/**
* AI Component that implements AI to any Actor
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINISHOOTER_API UMShooterAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMShooterAIComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	* Alert that Player within Patrol Zone
	*/
	UFUNCTION()
		void StartPlayerChase();

	/**
	* Alert that Player left Patrol Zone
	*/
	UFUNCTION()
		void StopPlayerChase();

	/**
	* Set Target Location
 	*
  	* @param NewLocation Vector that determines the new location
	*/
	UFUNCTION()
		void SetTargetLocation(FVector NewLocation);

	/**
	* Activate / Deactivate AI
 	*
  	* @param bActivate Used to Activate or Deactivate AI
	*/
	UFUNCTION()
		void SetAIBehaviour(bool bActivate);

protected:

	/**
	* State Machine for AI
	*/
	UPROPERTY()
		EStateMachine ActiveState {EStateMachine::Idle};

	/**
	* Previous State Track for AI
	*/
	UPROPERTY()
		EStateMachine PreviousState {EStateMachine::Idle};

	/**
	* Same State Timer Handle Tracker
	*/
	UPROPERTY()
		FTimerHandle InSameStateTimer;

	/**
	* Time in other state before Rest
	*/
	UPROPERTY(EditAnywhere, Category = "AI Config")
		float TimeBeforeResting{ 5.f };

	/**
	* Time of Resting
	*/
	UPROPERTY(EditAnywhere, Category = "AI Config")
		float TimeOfResting{ 3.f };

	/**
	*  Partrol Walking Speed
	*/
	UPROPERTY(EditAnywhere, Category = "AI Config")
		float PatrolWalkSpeed{ 2.f };

	/**
	*  Chasing player run Speed
	*/
	UPROPERTY(EditAnywhere, Category = "AI Config")
		float ChaseRunSpeed{ 4.f };

	/**
	*  Target Location
	*/
	UPROPERTY()
		FVector TargetLocation{ FVector::ZeroVector };

	/**
	* Change State method for the current AI functionality
 	*
  	* @param NewState AI State that we want to transition to
	*/
	UFUNCTION()
		void ChangeState(EStateMachine NewState);

	/**
	* State Functionality: Rest
	*/
	UFUNCTION()
		void StartRestFunctionality();

	/**
	* State Functionality: Patrol
	*/
	UFUNCTION()
		void StartPatrolFunctionality();

	/**
	* State Functionality: Chase
	*/
	UFUNCTION()
		void StartChaseFunctionality();

	/**
	* Method that moves actor if allowed by state
 	*
  	* @param DeltaTime Takes a DeltaTime value 
	*/
	UFUNCTION()
		void MoveActor(float DeltaTime);

	// Called when the game starts
	virtual void BeginPlay() override;
};
