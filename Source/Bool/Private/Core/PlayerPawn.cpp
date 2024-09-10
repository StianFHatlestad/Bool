// Fill out your copyright notice in the Description page of Project Settings.
#include "Core/PlayerPawn.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "Balls/CueBall.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


APlayerPawn::APlayerPawn()
{
	//enable ticking
	PrimaryActorTick.bCanEverTick = true;

	//create the camera component
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	//set the rotation of the camera component
	CameraComponent->SetRelativeRotation(FRotator(0,-90,0));
}

void APlayerPawn::BeginPlay()
{
	//call the parent implementation
	Super::BeginPlay();

	//get the player controller
	PlayerController = CastChecked<APlayerController>(GetController());

	//get the cue ball
	CurrentCueBall = Cast<ACueBall>(UGameplayStatics::GetActorOfClass(GetWorld(), ACueBall::StaticClass()));
}

void APlayerPawn::Tick(float DeltaTime)
{
	//call the parent implementation
	Super::Tick(DeltaTime);

	//check if the shooting trajectory is locked
	if (!bLockedShotTrajectory)
	{
		//set the aim location to the current mouse world position
		AimLocation = GetMouseWorldPosition();
	}
	else
	{
		//set the current shot speed
		CurrentShotSpeed = FMath::Clamp(FVector::Dist(CurrentCueBall->GetActorLocation(), GetMouseWorldPosition()) * ShotSpeedMultiplier, 0, MaxShootingSpeed);
	}
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//call the parent implementation
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//check if we have a valid input data asset and enhanced input component
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent); EnhancedInputComponent->IsValidLowLevel())
	{
		EnhancedInputComponent->BindAction(IA_ShootAction, ETriggerEvent::Triggered, this, &APlayerPawn::ShootCueBall);
	}

	//check if we have a valid input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalViewingPlayerController()->GetLocalPlayer()))
	{
		//add the input mapping context
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void APlayerPawn::ShootCueBall(const FInputActionValue& Value)
{
	//check if the shooting trajectory is locked
	if (!bLockedShotTrajectory)
	{
		//toggle locking the shooting trajectory
		ToggleLockedShootingTrajectory(Value);

		//return early to prevent further execution
		return;
	}

	//check if we have a valid cue ball
	if (CurrentCueBall)
	{
		//get the direction to shoot the cue ball
		const FVector Direction = (CurrentCueBall->GetActorLocation() - AimLocation).GetSafeNormal();

		//shoot the cue ball
		CurrentCueBall->SphereComponent->AddImpulse(Direction * CurrentShotSpeed, NAME_None, true);

		//toggle locking the shooting trajectory
		ToggleLockedShootingTrajectory(Value);
	}
}

void APlayerPawn::ToggleLockedShootingTrajectory(const FInputActionValue& Value)
{
	//toggle the locked shot trajectory
	bLockedShotTrajectory = !bLockedShotTrajectory;
}

FVector APlayerPawn::GetMouseWorldPosition()
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
	if (CurrentCueBall->IsValidLowLevelFast())
	{
		TrueWorldLocation.Z = CurrentCueBall->GetActorLocation().Z;
	}

	//return the world location
	return TrueWorldLocation;
}
