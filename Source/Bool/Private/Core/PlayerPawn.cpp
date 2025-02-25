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

	//set the player controller input mode
	PlayerController->SetInputMode(FInputModeGameAndUI());

	//set the player controller show mouse cursor
	PlayerController->bShowMouseCursor = true;

	//set the turns this round to the default turns per round
	TurnsThisRound = TurnsPerRound;

	//set the start location of the cue ball
	CueBallStartLocation = CueBall->GetActorLocation();
}

void APlayerPawn::Tick(const float DeltaTime)
{
	//call the parent implementation
	Super::Tick(DeltaTime);

	//check if the shooting trajectory is locked
	if (!bLockedShotTrajectory)
	{
		//set the aim location to the current mouse world position
		AimLocation = GetMouseWorldPosition();
	}

	//check if the turn is in progress and we can shoot (all balls are stopped)
	if (bTurnInProgress && CanShoot())
	{
		//call the OnTurnEnd function
		OnTurnEnd();

		//set turn in progress to false
		bTurnInProgress = false;
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
	//convert the location to a point on the cue ball
	const FVector LocationToHit = ConvertLocationToCueBall(CueBallHitLocation);

	//calculate the world center of mass
	const Chaos::FVec3 WorldCOM = CueBall->SphereComponent->GetComponentLocation() + CueBall->SphereComponent->GetComponentRotation().RotateVector(CueBall->SphereComponent->GetCenterOfMass());

	//calculate the angular impulse
	const Chaos::FVec3 AngularImpulse = Chaos::FVec3::CrossProduct(LocationToHit - WorldCOM, NewVelocity);

	//add the impulse to the cue ball
	CueBall->SetBallVelocity(NewVelocity);
	CueBall->SetBallAngularVelocity(AngularImpulse);
}

void APlayerPawn::SetCueBallHitLocation(const FVector2D HitLocation)
{
	//set the cue ball hit location
	CueBallHitLocation = HitLocation;
}

FVector APlayerPawn::ConvertLocationToCueBall(const FVector2D InLocation) const
{
	//the local aimlocation
	FVector LocAimLocation = FVector(AimLocation.X, AimLocation.Y, CueBall->GetActorLocation().Z);

	//direction from the aim location to the cue ball
	const FVector AimDirection = (CueBall->GetActorLocation() - LocAimLocation).GetSafeNormal();

	//storage for the hit result
	FHitResult HitResult;

	//do a line trace to get the hit result
	GetWorld()->LineTraceSingleByChannel(HitResult, LocAimLocation, LocAimLocation + AimDirection * 10000, ECC_Visibility);

	//check if the hit result is not valid
	if (!HitResult.IsValidBlockingHit())
	{
		//print a debug message
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No Hit Result Found"));

		//return early to prevent further execution
		return FVector::ZeroVector;
	}

	
	//get the x axis direction
	const FVector XAxisDirection = FVector::CrossProduct(AimDirection, FVector::UpVector).GetSafeNormal() * -1;

	//storage for the x axis location
	const FVector XAxisLocation = CueBall->SphereComponent->GetScaledSphereRadius() * XAxisDirection * InLocation.X;

	//get the y axis direction
	const FVector YAxisDirection = FVector::CrossProduct(AimDirection, FVector::RightVector).GetSafeNormal();

	//storage for the y axis location
	const FVector YAxisLocation = CueBall->SphereComponent->GetScaledSphereRadius() * YAxisDirection * InLocation.Y;

	//set the return value
	const FVector ReturnValue = CueBall->GetActorLocation() + XAxisLocation + YAxisLocation + HitResult.ImpactPoint - CueBall->GetActorLocation();

	//return the return result
	return ReturnValue;
	
}

bool APlayerPawn::CanShoot() const
{
	//check if we have a valid cue ball
	if (!CueBall->IsValidLowLevelFast())
	{
		//return false
		return false;
	}

	//check if enough time hasn't passed since last turn
	if (GetWorld()->GetTimeSeconds() < AvailableTurnTime)
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
		//check if the physics linear velocity is greater than the ball stop speed
		if (Cast<ABallActor>(BallActor)->GetBallVelocity().Size() > CueBall->StationarySpeed)
		{
			//return false
			return false;
		}
	}

	//check if the cue ball actors' physics linear velocity is greater than the ball stop speed
	if (CueBall->GetBallVelocity().Size() > CueBall->StationarySpeed)
	{
		//return false
		return false;
	}

	//return true
	return true;
}

void APlayerPawn::ShootCueBall(const FInputActionValue& Value)
{
	//check if there is a turn in progress or if we're not allowed to play or if we can't shoot given current conditions
	if (bTurnInProgress || !bCanPlay || !CanShoot())
	{
		//return early to prevent further execution
		return;
	}

	//check if the shooting trajectory is locked
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
		//get the direction to shoot the cue ball
		const FVector Direction = (CueBall->GetActorLocation() - AimLocation).GetSafeNormal();

		//get the current shot speed
		float LocCurrentShotSpeed = FMath::Clamp(FVector::Dist(CueBall->GetActorLocation(), GetMouseWorldPosition()) * ShotSpeedMultiplier, MinimumShootingSpeed, MaxShootingSpeed);

		//print the current shot speed
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Blue, FString::SanitizeFloat(LocCurrentShotSpeed));

		//shoot the cue ball at the position
		ShootCueBallAtPosition(Direction * LocCurrentShotSpeed, NAME_None);

		//set turn in progress to true
		bTurnInProgress = true;

		//set the next available turn time
		AvailableTurnTime = GetWorld()->GetTimeSeconds() + MinTurnTime;

		//toggle locking the shooting trajectory
		bLockedShotTrajectory = !bLockedShotTrajectory;
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
	else
	{
		//print debug message
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("no valid cue ball"));
	}

	//return the world location
	return TrueWorldLocation;
}

bool APlayerPawn::HandleBallInGoal(AGoalActor* Goal, AActor* BallActor)
{
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
			PlayerScore += Ball->CurrentTurnData->ScoreToGive;

			//add the gold to the player gold
			PlayerGold += Ball->CurrentTurnData->GoldToGive;
		}

		//destroy the ball actor
		Ball->Destroy();
	}
	else
	{
		//get the cue ball
		const TObjectPtr<ABallActor> LocCueBall = Cast<ABallActor>(Ball);

		//set the location of the cue ball back to the start position
		LocCueBall->SetActorLocation(CueBallStartLocation);

		//set the linear velocity to zero
		LocCueBall->SphereComponent->SetAllPhysicsLinearVelocity(FVector::Zero());

		//set the angular velocity to zero
		LocCueBall->SphereComponent->SetAllPhysicsAngularVelocityInRadians(FVector::Zero());
	}

	return true;
}

void APlayerPawn::OnTurnEnd()
{
	//add on screen debug message
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Turn Ended. Current Turn: " + FString::FromInt(CurrentTurn)));

	//increment the current turn
	CurrentTurn++;

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

	//check if the current turn is greater than or equal to the turns this round
	if (CurrentTurn >= TurnsThisRound)
	{
		//call the OnRoundEnd function
		OnRoundEnd();
	}
}

void APlayerPawn::OnRoundEnd()
{
	//set the current turn to 0
	CurrentTurn = 0;

	//set can play to false
	bCanPlay = false;

	//call the blueprint OnRoundEnd function
	OnRoundEndBP();
}
