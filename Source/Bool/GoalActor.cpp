// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalActor.h"

#include "Balls/BallActor.h"
#include "Components/BoxComponent.h"
#include "Core/PlayerPawn.h"
#include "Kismet/GameplayStatics.h"


AGoalActor::AGoalActor()
{
	//create the box component
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));

	//set the box component to be the root component
	SetRootComponent(BoxComponent);

	//set parameters for the box component
	BoxComponent->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	BoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetCanEverAffectNavigation(false);

	//bind the overlap events
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AGoalActor::OnBoxBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AGoalActor::OnBoxEndOverlap);
}

void AGoalActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//check if the other actor is a ball actor
	if (OtherActor->IsA(ABallActor::StaticClass()))
	{
		//get the ball actor
		ABallActor* BallActor = Cast<ABallActor>(OtherActor);

		//get the player pawns in the world
		TArray<AActor*> PlayerPawns;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerPawn::StaticClass(), PlayerPawns);

		//cast the first player pawn
		APlayerPawn* PlayerPawn = Cast<APlayerPawn>(PlayerPawns[0]);

		//check if the playerpawn is not valid
		if (!PlayerPawn->IsValidLowLevelFast())
		{
			//print a debug message
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No PlayerPawn Found"));

			//return early to prevent further execution
			return;
		}

		//check if the ball is a cue ball
		if (BallActor->ActorHasTag(PlayerPawn->CueBallTag))
		{
			//set the cue balls velocity to zero
			BallActor->SetBallVelocity(FVector::ZeroVector);

			//set the cue balls angular velocity to zero
			BallActor->SetBallAngularVelocity(FVector::ZeroVector);
		}
		else
		{
			//call the player pawns handle ball in goal function
			PlayerPawn->HandleBallInGoal(this, BallActor);
		}
	}
}

void AGoalActor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

