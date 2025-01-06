// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalActor.h"

#include "Balls/BallActor.h"
#include "Balls/BallUpgradeDataAsset.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"


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
	////check if the other actor is a ball actor
	//if (OtherActor->IsA(ABallActor::StaticClass()))
	//{
	//	//get the ball actor
	//	ABallActor* BallActor = Cast<ABallActor>(OtherActor);

	//	//set the ball actor's sphere to no longer be constrained
	//	BallActor->SphereComponent->SetConstraintMode(EDOFMode::Type::None);

	//	////print debug message to the screen
	//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ball in goal!"));
	//}
}

void AGoalActor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	////check if the other actor is a ball actor
	//if (OtherActor->IsA(ABallActor::StaticClass()))
	//{
	//	//get the ball actor
	//	ABallActor* BallActor = Cast<ABallActor>(OtherActor);

	//	//set the ball actor's sphere to be constrained to the XY plane
	//	BallActor->SphereComponent->SetConstraintMode(EDOFMode::Type::XYPlane);

	//	////print debug message to the screen
	//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Ball out of goal!"));
	//}
}

