// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "PlayerPawn.generated.h"

UCLASS()
class BOOL_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	//the camera component for this pawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* CameraComponent = nullptr;

	//the input mapping context for this pawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inputsystem")
	UInputMappingContext* InputMappingContext = nullptr;

	//the input action for shooting the ball
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inputsystem")
	UInputAction* IA_Shoot = nullptr;

	//the input action for resetting the aim of the ball
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inputsystem")
	UInputAction* IA_ResetAim = nullptr;

	//bool for checking if the cue ball trajectory is being set
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bLockedShotTrajectory = false;

	//the shot speed multipler
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShotSpeedMultiplier = 2;

	//the max speed at which the cue ball can be shot
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxShootingSpeed = 16000;

	//the minimum speed at which the cue ball can be shot
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinimumShootingSpeed = 4000;

	//the speed the balls velocity must be below before the player can shoot
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BallStopSpeed = 50;

	//whether or not the player can shoot right now
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanPlay = true;

	//the position on the ball that the player is going to hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D CueBallHitLocation = FVector2D::Zero();

	//the default amount of turns the player has each round
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Turns/Rounds")
	int TurnsPerRound = 5;

	//the current turn
	UPROPERTY(BlueprintReadOnly, Category="Turns/Rounds")
	int CurrentTurn = 1;

	//the amount of turns the player has left this round
	UPROPERTY(BlueprintReadOnly, Category="Turns/Rounds")
	int TurnsThisRound = 5;

	//the current speed at which the cue ball will be shot
	UPROPERTY(BlueprintReadOnly)
	float CurrentShotSpeed = 0;

	//the current location the player is aiming from
	UPROPERTY(BlueprintReadOnly)
	FVector AimLocation = FVector::ZeroVector;

	//whether or not a turn is currently in progress
	UPROPERTY(BlueprintReadOnly)
	bool bTurnInProgress = false;

	//the current cue ball we're using
	UPROPERTY(BlueprintReadOnly)
	class ACueBall* CurrentCueBall = nullptr;

	//reference to the player controller
	UPROPERTY()
	APlayerController* PlayerController = nullptr;

	//whether or not we've ended this turn
	bool bHasEndedTurn = false;

	//constructor(s)
	APlayerPawn();

	//override(s)
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//function to set the current cue ball hit location
	UFUNCTION(BlueprintCallable)
	void SetCueBallHitLocation(FVector2D HitLocation);

	//function to shoot the cue ball at a specific position
	UFUNCTION()
	void ShootCueBallAtPosition(FVector NewVelocity, FName BoneName);

	//function to convert the fvector2d location to a point on the cue ball
	UFUNCTION(BlueprintCallable)
	FVector ConvertLocationToCueBall(FVector2D InLocation);

	//function to check if we can shoot
	UFUNCTION(BlueprintCallable)
	bool CanShoot() const;

	//input function for shooting the cue ball
	UFUNCTION(BlueprintCallable)
	void ShootCueBall(const FInputActionValue& Value);

	//function for resetting the aim for shooting the cue ball
	UFUNCTION(BlueprintCallable)
	void ResetAim(const FInputActionValue& Value);

	//function for toggling the locking of the shoot trajectory
	UFUNCTION(BlueprintCallable)
	void ToggleLockedShootingTrajectory();

	//function for getting the world position of the mouse cursor
	UFUNCTION(BlueprintCallable)
	FVector GetMouseWorldPosition();

	//function called when the turn ends
	UFUNCTION()
	void OnTurnEnd();

	//function called when the round ends
	UFUNCTION()
	void OnRoundEnd();

	//blueprint events

	//event called when the turn ends
	UFUNCTION(BlueprintImplementableEvent)
	void OnTurnEndBP();

	//event called when the round ends
	UFUNCTION(BlueprintImplementableEvent)
	void OnRoundEndBP();
};
