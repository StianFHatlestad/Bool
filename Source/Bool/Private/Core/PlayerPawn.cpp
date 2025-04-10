// Fill out your copyright notice in the Description page of Project Settings.
#include "Core/PlayerPawn.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "Balls/BallActor.h"
#include "Balls/BallUpgradeDataAsset.h"
#include "Balls/BallCurrentTurnData.h"
#include "Bool/GoalActor.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Chaos/Particle/ParticleUtilities.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BoolGameInstance.h"
#include "GameFramework/PhysicsVolume.h"


APlayerPawn::APlayerPawn()
{
	//enable ticking
	PrimaryActorTick.bCanEverTick = true;

	//create the camera component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	//setup attachment(s)
	CameraComponent->SetupAttachment(RootComponent);

	//set the rotation of the camera component
	CameraComponent->SetRelativeRotation(FRotator(0,-90,0));
}

void APlayerPawn::BeginPlay()
{
	//call the parent implementation
	Super::BeginPlay();

	//set the player controller
	PlayerController = CastChecked<APlayerController>(GetController());

	//get the cue balls in the level
	TArray<AActor*> CueBalls;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ABallActor::StaticClass(), CueBallTag, CueBalls);

	//check if we don't have any cue balls
	if (CueBalls.IsEmpty())
	{
		//print a debug message
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Cue Balls Found"));

		//return early to prevent further execution
		return;
	}

	//set the cue ball
	CueBall = Cast<ABallActor>(CueBalls[0]);

	//set the player controller show mouse cursor
	PlayerController->bShowMouseCursor = true;

	//get the game instance
	GameInstance = Cast<UBoolGameInstance>(UGameplayStatics::GetGameInstance(this));

	//temporary storage for the ball actors in the level
	TArray<AActor*> LocBallActors;

	//get all the ball actors in the level
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABallActor::StaticClass(), LocBallActors);

	//iterate over the ball actors
	for (AActor* BallActor : LocBallActors)
	{
		//cast the ball actor to a ball actor
		const TObjectPtr<ABallActor> Ball = Cast<ABallActor>(BallActor);

		//check if the ball is valid
		if (Ball->IsValidLowLevel())
		{
			//add the ball to the level ball actors
			LevelBallActors.Add(Ball);
		}
	}
}

void APlayerPawn::Tick(const float DeltaTime)
{
	//call the parent implementation
	Super::Tick(DeltaTime);

	//empty the level actors array
	LevelBallActors.Empty();

	//temporary storage for the ball actors in the level
	TArray<AActor*> LocBallActors;

	//get all the ball actors in the level
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABallActor::StaticClass(), LocBallActors);

	//iterate over the ball actors
	for (AActor* BallActor : LocBallActors)
	{
		//cast the ball actor to a ball actor
		const TObjectPtr<ABallActor> Ball = Cast<ABallActor>(BallActor);

		//check if the ball is valid
		if (Ball->IsValidLowLevel())
		{
			//add the ball to the level ball actors
			LevelBallActors.Add(Ball);
		}
	}

	//check if the shooting trajectory is locked
	if (!bLockedShotTrajectory)
	{
		//set the aim location to the current mouse world position
		AimLocation = GetMouseWorldPosition();
	}

	//check if the game instance is not valid
	if (!GameInstance->IsValidLowLevel())
	{
		//return early to prevent further execution
		return;
	}

	//check if the turn is in progress and we can shoot (all balls are stopped)
	if (GameInstance->bTurnInProgress && CanShoot())
	{
		//call the OnTurnEnd function
		OnTurnEnd();

		//set turn in progress to false
		GameInstance->bTurnInProgress = false;
	}
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//call the parent implementation
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//check if we have a valid input data asset and enhanced input component
	if (const TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent); EnhancedInputComponent->IsValidLowLevel())
	{
		EnhancedInputComponent->BindAction(IA_Shoot, ETriggerEvent::Triggered, this, &APlayerPawn::ShootCueBall);
		EnhancedInputComponent->BindAction(IA_ResetAim, ETriggerEvent::Triggered, this, &APlayerPawn::ResetAim);
	}

	//check if we have a valid input subsystem
	if (const TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalViewingPlayerController()->GetLocalPlayer()))
	{
		//add the input mapping context
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void APlayerPawn::ShootCueBallAtPosition(FVector NewVelocity, const FName BoneName) const
{
	//check if we don't have a valid deflection float curve and shot strength curve and spin strength curve
	if (!(DeflectionCurve->IsValidLowLevelFast() && ShotStrengthCurve->IsValidLowLevelFast() && SpinStrengthCurve->IsValidLowLevelFast()))
	{
		//print debug message to the screen
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("APlayerPawn::ShootCueBallAtPosition No valid Deflection Curve, Shot Strength Curve, or Spin Strength Curve"));

		return;
	}

	//get the vector from the aim location to the cue ball location normalized
	const FVector AimToCueBall = (CueBall->GetActorLocation() - AimLocation).GetSafeNormal();

	//get a vector perpendicular to the aim to cue ball vector
	const FVector PerpendicularVector = FVector(AimToCueBall.Y, -AimToCueBall.X, 0);

	//get the deflection float curve value
	const float DeflectionFloatCurveValue = DeflectionCurve->GetFloatValue(CueBallHitLocation.X);

	//get the shot strength curve value
	const float ShotStrengthCurveValue = ShotStrengthCurve->GetFloatValue(FVector::DotProduct(AimToCueBall, NewVelocity.GetSafeNormal()));

	//get the spin strength curve value
	const float SpinStrengthCurveValue = SpinStrengthCurve->GetFloatValue(NewVelocity.Length() / GetWorld()->GetDefaultPhysicsVolume()->TerminalVelocity);

	//storage for the new velocity of the cue ball
	const FVector LocNewVelocity = (NewVelocity.GetSafeNormal() * (1 - DeflectionFloatCurveValue) + PerpendicularVector * DeflectionFloatCurveValue) * NewVelocity.Size() * ShotStrengthCurveValue;

	//storage for the pitch component of the angular velocity to give the cue ball
	float Pitch = FVector::DotProduct(-NewVelocity.GetSafeNormal(), CueBall->SphereComponent->GetForwardVector()) * CueBallHitLocation.Y;

	//storage for the side spin component of the pitch
	float PitchSideSpin = FVector::DotProduct(NewVelocity.GetSafeNormal(), CueBall->SphereComponent->GetForwardVector()) * -CueBallHitLocation.X * FMath::Sign(FVector::DotProduct(NewVelocity.GetSafeNormal(), CueBall->SphereComponent->GetForwardVector()));

	//storage for the roll component of the angular velocity to give the cue ball
	float Roll = FVector::DotProduct(NewVelocity.GetSafeNormal(), CueBall->SphereComponent->GetRightVector()) * CueBallHitLocation.Y;

	//storage for the side spin component of the roll
	float RollSideSpin = FVector::DotProduct(NewVelocity.GetSafeNormal(), CueBall->SphereComponent->GetRightVector()) * -CueBallHitLocation.X * FMath::Sign(FVector::DotProduct(NewVelocity.GetSafeNormal(), CueBall->SphereComponent->GetRightVector()));

	//the angular rotaion to give the cue ball
	FRotator OutPutAngularVelocity = FRotator(Pitch, PitchSideSpin + RollSideSpin, Roll) * SpinStrengthCurveValue * NewVelocity.Length();

	//add the impulse to the cue ball
	CueBall->SetBallVelocity(LocNewVelocity);
	CueBall->SetBallAngularVelocity(OutPutAngularVelocity);
}

void APlayerPawn::SetCueBallHitLocation(const FVector2D HitLocation)
{
	//set the cue ball hit location
	CueBallHitLocation = HitLocation;
}

bool APlayerPawn::CanShoot() const
{
	//check if we have a valid cue ball
	if (!CueBall->IsValidLowLevelFast())
	{
		//return false
		return false;
	}

	//check if the game instance is not valid
	if (!GameInstance->IsValidLowLevel())
	{
		//return early to prevent further execution
		return false;
	}

	//check if enough time hasn't passed since last turn
	if (GetWorld()->GetTimeSeconds() < GameInstance->AvailableTurnTime)
	{
		//return false
		return false;
	}

	//get all ball actors in the world
	TArray<AActor*> BallActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABallActor::StaticClass(), BallActors);

	//loop through the ball actors
	for (TObjectPtr<AActor> BallActor : BallActors)
	{
		//check if the ball is not stationary
		if (Cast<ABallActor>(BallActor)->PhysicsState != Ebps_Stationary)
		{
			//return false
			return false;
		}
	}

	//check if the cue ball is not stationary
	if (CueBall->PhysicsState != Ebps_Stationary)
	{
		//return false
		return false;
	}

	//return true
	return true;
}

void APlayerPawn::ShootCueBall(const FInputActionValue& Value)
{
	//check if the game instance is not valid
	if (!GameInstance->IsValidLowLevel())
	{
		//print debug string
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("APlayerPawn::ShootCueBall Game Instance Not Valid"));

		//return early to prevent further execution
		return;
	}

	//check if there is a turn in progress or if we're not allowed to play or if we can't shoot given current conditions
	if (GameInstance->bTurnInProgress || !bCanPlay || !CanShoot())
	{
		//return early to prevent further execution
		return;
	}

	//check if the shooting trajectory is not locked
	if (!bLockedShotTrajectory)
	{
		//toggle locking the shooting trajectory
		bLockedShotTrajectory = !bLockedShotTrajectory;

		//return early to prevent further execution
		return;
	}

	//check if we have a valid cue ball
	if (CueBall->IsValidLowLevelFast())
	{
		//the timer handle for the shot delay
		FTimerHandle ShotDelayTimerHandle;

		//check if the shot delay is less than or equal to zero
		if (ShotDelay <= 0)
		{
			//get the direction to shoot the cue ball
			const FVector Direction = (CueBall->GetActorLocation() - AimLocation).GetSafeNormal();

			//get the current shot speed
			float LocCurrentShotSpeed = FMath::Clamp(FVector::Dist(CueBall->GetActorLocation(), GetMouseWorldPosition()) * ShotSpeedMultiplier, MinimumShootingSpeed, MaxShootingSpeed);

			//shoot the cue ball at the position
			ShootCueBallAtPosition(Direction * LocCurrentShotSpeed, NAME_None);

			//set turn in progress to true
			GameInstance->bTurnInProgress = true;

			//set the next available turn time
			GameInstance->AvailableTurnTime = GetWorld()->GetTimeSeconds() + GameInstance->MinTurnTime;

			//toggle locking the shooting trajectory
			bLockedShotTrajectory = !bLockedShotTrajectory;
		}
		else
		{
			//set lambda function to shoot the cue ball after the shot delay
			GetWorld()->GetTimerManager().SetTimer(ShotDelayTimerHandle, [this]
			{
				//get the direction to shoot the cue ball
				const FVector Direction = (CueBall->GetActorLocation() - AimLocation).GetSafeNormal();

				//get the current shot speed
				float LocCurrentShotSpeed = FMath::Clamp(FVector::Dist(CueBall->GetActorLocation(), GetMouseWorldPosition()) * ShotSpeedMultiplier, MinimumShootingSpeed, MaxShootingSpeed);

				////print the current shot speed
				//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Blue, FString::SanitizeFloat(LocCurrentShotSpeed));

				//shoot the cue ball at the position
				ShootCueBallAtPosition(Direction * LocCurrentShotSpeed, NAME_None);

				//set turn in progress to true
				GameInstance->bTurnInProgress = true;

				//set the next available turn time
				GameInstance->AvailableTurnTime = GetWorld()->GetTimeSeconds() + GameInstance->MinTurnTime;

				//toggle locking the shooting trajectory
				bLockedShotTrajectory = !bLockedShotTrajectory;
			}, ShotDelay, false);	
		}
	}
}

void APlayerPawn::ResetAim(const FInputActionValue& Value)
{
	//set the locket shot trajectory
	bLockedShotTrajectory = false;
}

FVector APlayerPawn::GetMouseWorldPosition() const
{
	//get the mouse position
	float MouseX;
	float MouseY;
	PlayerController->GetMousePosition(MouseX, MouseY);

	//convert the mouse position to a world location
	FVector WorldLocation;
	FVector WorldDirection;
	PlayerController->DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection);

	//convert the location to actual world location
	FVector TrueWorldLocation = WorldLocation.Z / WorldDirection.Z * WorldDirection * -1 + WorldLocation;

	//check if we have a valid cue ball
	if (CueBall->IsValidLowLevelFast())
	{
		TrueWorldLocation.Z = CueBall->GetActorLocation().Z;
	}

	//return the world location
	return TrueWorldLocation;
}

bool APlayerPawn::HandleBallInGoal(AGoalActor* Goal, AActor* BallActor)
{
	//check if the game instance is not valid
	if (!GameInstance->IsValidLowLevel())
	{
		//print debug string
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("APlayerPawn::HandleBallInGoal Game Instance Not Valid"));

		//return early to prevent further execution
		return false;
	}

	//check if the ball actor is not valid or if the ball actor is not a ball actor
	if (!BallActor->IsValidLowLevelFast() || !BallActor->IsA(ABallActor::StaticClass()))
	{
		//print the static class to the screen
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, ABallActor::StaticClass()->GetName());

		//return early to prevent further execution
		return false;
	}

	//get the ball actor
	const TObjectPtr<ABallActor> Ball = Cast<ABallActor>(BallActor);

	//iterate through the ball upgrade data assets
	for (const auto BallUpgradeDataAsset : Ball->BallUpgradeDataAssets)
	{
		//call the OnGoalHit function
		BallUpgradeDataAsset.Get()->GetDefaultObject<UBallUpgradeDataAsset>()->OnGoal(Ball, Goal);
	}

	//check if the object is not a cueball
	if (!Ball->ActorHasTag(CueBallTag))
	{
		//check if the ball has a current turn data
		if (Ball->CurrentTurnData)
		{
			//add the score to the player score
			GameInstance->PlayerScore += Ball->CurrentTurnData->ScoreToGive;

			//add the gold to the player gold
			GameInstance->PlayerGold += Ball->CurrentTurnData->GoldToGive;
		}

		//destroy the ball actor
		Ball->Destroy();
	}
	else
	{
		//get the cue ball
		const TObjectPtr<ABallActor> LocCueBall = Cast<ABallActor>(Ball);

		//set the location of the cue ball back to the start position
		LocCueBall->SetActorLocation(LocCueBall->StartLocation);

		//set the linear velocity to zero
		LocCueBall->SphereComponent->SetAllPhysicsLinearVelocity(FVector::Zero());

		//set the angular velocity to zero
		LocCueBall->SphereComponent->SetAllPhysicsAngularVelocityInRadians(FVector::Zero());
	}

	return true;
}

void APlayerPawn::OnTurnEnd()
{
	//check if the game instance is not valid
	if (!GameInstance->IsValidLowLevel())
	{
		//print debug string
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("APlayerPawn::OnTurnEnd Game Instance Not Valid"));

		//return early to prevent further execution
		return;
	}

	////add on screen debug message
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Turn Ended. Current Turn: " + FString::FromInt(GameInstance->CurrentTurn)));

	//increment the current turn
	GameInstance->CurrentTurn++;

	//get all goal actors in the world
	TArray<AActor*> GoalActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGoalActor::StaticClass(), GoalActors);

	//iterate through the goal actors
	for (TObjectPtr<AActor> GoalActor : GoalActors)
	{
		//get the goal actor
		TObjectPtr<AGoalActor> Goal = Cast<AGoalActor>(GoalActor);

		//get all ball actors in the goal
		TArray<AActor*> BallActors;
		Goal->BoxComponent->GetOverlappingActors(BallActors);

		//iterate through the ball actors
		for (TObjectPtr<AActor>BallActor : BallActors)
		{
			//call the HandleBallInGoal function
			HandleBallInGoal(Goal, BallActor);
		}
	}

	//call the blueprint OnTurnEnd function
	OnTurnEndBP();

	//call the OnTurnEnd function of the game instance
	GameInstance->OnTurnEndBP();
}
