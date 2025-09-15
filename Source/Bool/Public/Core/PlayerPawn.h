// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Bool/GoalActor.h"
#include "Timelord.h"
#include "PlayerPawn.generated.h"

class ABallActor;
class UBoolGameInstance;

UCLASS()
class BOOL_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	//the camera component for this pawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCameraComponent> CameraComponent = nullptr;
	/*
	//Niagara particle system component for visual effects
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UNiagaraComponent> NS_AimingRing{nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UNiagaraComponent> NS_AimingLine{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UNiagaraComponent> NS_AimingLineBounces{ nullptr };
	*/
	//the current cue ball we're using
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ABallActor> CueBall = nullptr;

	//reference to the player controller
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController = nullptr;

	//our game instace
	UPROPERTY()
	TObjectPtr<UBoolGameInstance> GameInstance = nullptr;

	//the input mapping context for this pawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inputsystem")
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;

	//the input action for shooting the ball
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inputsystem")
	TObjectPtr<UInputAction> IA_Shoot = nullptr;

	//the input action for rewind. TODO: remove this and make it a button on UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inputsystem")
	TObjectPtr<UInputAction> IA_Rewind = nullptr;

	//toggle for whether the player fires the ball in the direction of the mouse cursor or the opposite direction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData")
	bool bFireInMouseDir = true;

	//storage for the direction we're firing in
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData")
	FVector FireDir = FVector::Zero();

	//the input action for resetting the aim of the ball
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inputsystem")
	TObjectPtr<UInputAction> IA_ResetAim = nullptr;

	//the deflection float curve for hitting the cue ball
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData")
	UCurveFloat* DeflectionCurve = nullptr;

	//the float curve for the strength of the shot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData")
	UCurveFloat* ShotStrengthCurve = nullptr;
	
	//the spin strength float curve for hitting the cue ball
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData")
	UCurveFloat* SpinStrengthCurve = nullptr;

	//the tag for the cue ball
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData")
	FName CueBallTag = FName("CueBallTag");

	//bool for checking if the cue ball trajectory is being set
	UPROPERTY(BlueprintReadOnly, Category="BoolData")
	bool bLockedShotTrajectory = false;

	//the amount of delay before the shot happens (for animations)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData")
	float ShotDelay = 0;

	//the shot speed multipler
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BoolData")
	float ShotSpeedMultiplier = 2;

	//the max speed at which the cue ball can be shot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BoolData")
	float MaxShootingSpeed = 16000;

	//the minimum speed at which the cue ball can be shot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BoolData")
	float MinimumShootingSpeed = 4000;

	//whether or not the player can shoot right now
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BoolData")
	bool bCanPlay = true;

	//the position on the ball that the player is going to hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BoolData")
	FVector2D CueBallHitLocation = FVector2D::Zero();

	//the current location the player is aiming from
	UPROPERTY(BlueprintReadOnly)
	FVector AimLocation = FVector::ZeroVector;

	//storage for all of the balls in the level
	UPROPERTY(BlueprintReadWrite)
	TArray<ABallActor*> LevelBallActors;

	//the current projected shot strength
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BoolData")
	float CurrentShotStrength = 0;

	//the timer handle for the shot delay
	UPROPERTY()
	FTimerHandle ShotDelayTimerHandle;

	//Rewind Controller
	TObjectPtr<ATimelord> RewindController{ nullptr };
	//constructor(s)
	APlayerPawn();

	//override(s)
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//function to shoot the cue ball at a specific position
	UFUNCTION(BlueprintCallable)
	void ShootCueBallAtPosition(FVector NewVelocity) const;

	//function to set the current cue ball hit location
	UFUNCTION(BlueprintCallable)
	void SetCueBallHitLocation(FVector2D HitLocation);

	//function to check if we can shoot
	UFUNCTION(BlueprintCallable)
	bool CanShoot() const;

	//function to impart the force to the cue ball
	UFUNCTION(BlueprintCallable)
	void LaunchCueBall();

	//input function for shooting the cue ball
	UFUNCTION(BlueprintCallable)
	void ShootCueBall(const FInputActionValue& Value);

	//function for resetting the aim for shooting the cue ball
	UFUNCTION(BlueprintCallable)
	void ResetAim(const FInputActionValue& Value);

	//function for getting the world position of the mouse cursor
	UFUNCTION(BlueprintCallable)
	FVector GetMouseWorldPosition() const;

	UFUNCTION(BlueprintCallable)
	bool HandleBallInGoal(AGoalActor* Goal, AActor* BallActor);

	//function called when the turn ends
	UFUNCTION()
	void OnTurnEnd();

	//blueprint events 

	//event called when the cue ball is shot
	UFUNCTION(BlueprintImplementableEvent)
	void OnCueBallShotBP();

	//event called when the turn ends
	UFUNCTION(BlueprintImplementableEvent)
	void OnTurnEndBP();


	//Initiate rewind
	UFUNCTION(BlueprintCallable)
	void Rewind();
	/*
	UFUNCTION()
	void DrawBoolPlayerDebugArrows();*/
};	  
	  