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

	//class components

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

	//the current speed at which the cue ball will be shot
	UPROPERTY(BlueprintReadOnly)
	float CurrentShotSpeed = 0;

	//the current location the player is aiming from
	UPROPERTY(BlueprintReadOnly)
	FVector AimLocation = FVector::ZeroVector;

	//the current cue ball we're using
	UPROPERTY(BlueprintReadOnly)
	class ACueBall* CurrentCueBall = nullptr;

	//reference to the player controller
	UPROPERTY()
	APlayerController* PlayerController = nullptr;

	//constructor(s)
	APlayerPawn();

	//override(s)
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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

};
