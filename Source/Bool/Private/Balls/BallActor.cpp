// Fill out your copyright notice in the Description page of Project Settings.


#include "Balls/BallActor.h"

#include <string>

#include "Balls/BallUpgradeDataAsset.h"
#include "Balls/CueBall.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"


ABallActor::ABallActor()
{
	////enable ticking
	//PrimaryActorTick.bCanEverTick = true;

	//create the component(s)
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	//set the sphere component to be the root component
	SetRootComponent(SphereComponent);

	//setup attachment(s)
	MeshComponent->SetupAttachment(RootComponent);

	////set parameters for the sphere component
	//SphereComponent->InitSphereRadius(50.0f);
	//SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	//SphereComponent->SetSimulatePhysics(true);
	//SphereComponent->SetLinearDamping(0.1f);
	//SphereComponent->SetAngularDamping(0.1f);
	//SphereComponent->SetEnableGravity(true);
	////SphereComponent->SetConstraintMode(EDOFMode::Type::XYPlane);
	//SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//SphereComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	//bind the OnHit event
	SphereComponent->OnComponentHit.AddDynamic(this, &ABallActor::OnSphereHit);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABallActor::OnSphereOverlap);
}

void ABallActor::UpdateOldVelocities()
{
	//add the old velocity
	OldVelocities.Add(CurrentVelocity);

	//check if we have more than 10 old velocities
	if (OldVelocities.Num() > 10)
	{
		//remove the first element
		OldVelocities.RemoveAt(0);
	}
}

void ABallActor::Tick(const float DeltaTime)
{
	//call the parent implementation
	Super::Tick(DeltaTime);

	//check if we're not using custom physics
	if (!bUseCustomPhysics)
	{
		//update the old velocities
		UpdateOldVelocities();

		//return early to prevent further execution
		return;
	}

	////update the current velocity variable
	//CurrentVelocity = SphereComponent->GetPhysicsLinearVelocity();

	////update the angular velocity
	//AngularVelocity = SphereComponent->GetPhysicsAngularVelocityInRadians();

	//update the bool physics state
	UpdateBoolPhysicsState(DeltaTime);

	//update the bool physics variables
	UpdatePhysicsVariables(DeltaTime);

	//update the old velocity
	UpdateOldVelocities();

	//check if we're not using custom collision response
	if (!bUseCustomCollisionResponse)
	{
		//return early to prevent further execution
		return;
	}

	//get all actors with the ignore collision tag
	TArray<AActor*> ActorsToIgnore;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), ActorTagToIgnore, ActorsToIgnore);

	//iterate over the actors to ignore
	for (AActor* ActorToIgnore : ActorsToIgnore)
	{
		//ignore the actor
		SphereComponent->IgnoreActorWhenMoving(ActorToIgnore, true);
	}

	//storage for the hit result
	FHitResult Hit;

	//storage for the delta velocity
	DeltaVelocity = GetBallVelocity() * DeltaTime;

	//check if the delta velocity is near 0
	if (DeltaVelocity.IsNearlyZero() && bDebugMode)
	{
		//print debug statement
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.f, FColor::Blue, "DeltaVelocity Is Zero");

		//return early to prevent further execution
		return;
	}

	//move the component
	SphereComponent->MoveComponent(GetBallVelocity() * DeltaTime, SphereComponent->GetComponentRotation(), true, &Hit);

	//check if we hit anything
	if (Hit.IsValidBlockingHit() && bDebugMode)
	{
		//print the name of the actor
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Blue, Hit.GetActor()->GetActorNameOrLabel());
	}
}

void ABallActor::UpdateBoolPhysicsState(const float DeltaTime)
{
	////check if we're spinning
	//if (DeltaTime <= 2 * SphereComponent->GetScaledSphereRadius() / (5 * TableSpinningFrictionCoefficient * (GetWorld()->GetGravityZ() / 100)) * AngularVelocity.Z)
	//{
	//	//set the physics state to spinning
	//	SetBoolPhysicsState(Ebps_Spinning, DeltaTime);

	//	//return to prevent further execution
	//	return;
	//}

	////check if we're basically stationary
	//if (SphereComponent->GetPhysicsLinearVelocity().Length() < StationarySpeed)
	//{
	//	//set the physics state to stationary
	//	SetBoolPhysicsState(Ebps_Stationary, DeltaTime);

	//	//return to prevent further execution
	//	return;
	//}

	////check if there is a large enough difference between the balls linear velocity and the velocity at the point of contact
	//if (const FVector VelocityAtPoint = SphereComponent->GetPhysicsLinearVelocityAtPoint(SphereComponent->GetComponentLocation() - FVector(0, 0, SphereComponent->GetScaledSphereRadius())); (SphereComponent->GetPhysicsLinearVelocity() - VelocityAtPoint).Length() > SlidingRollingThreshold)
	//{
	//	//set the physics state to sliding
	//	SetBoolPhysicsState(Ebps_Sliding, DeltaTime);

	//	//return to prevent further execution
	//	return;
	//}

	////set the physics state to rolling
	//SetBoolPhysicsState(Ebps_Rolling, DeltaTime);

	//check if we're basically stationary
	if (!SphereComponent->GetComponentVelocity().IsNearlyZero() && AngularVelocity.IsNearlyZero())
	{
		//set the physics state to stationary
		SetBoolPhysicsState(Ebps_Stationary);

		//return to prevent further execution
		return;
	}

	//check if we're spinning
	if (DeltaTime <= 2 * SphereComponent->GetScaledSphereRadius() / (5 * TableSpinningFrictionCoefficient * (GetWorld()->GetGravityZ() / 100)) * AngularVelocity.Z)
	{
		//set the physics state to spinning
		SetBoolPhysicsState(Ebps_Spinning);

		//return to prevent further execution
		return;
	}

	//check if we're sliding
	if (DeltaTime <= /*2 / 7 **/ InitialRelativeVelocity.Length() / TableSlidingFrictionCoefficient * (GetWorld()->GetGravityZ() / 100))
	{
		//set the physics state to sliding
		SetBoolPhysicsState(Ebps_Sliding);

		//return to prevent further execution
		return;
	}

	//check if we're rolling
	if (DeltaTime <= CurrentVelocity.Length() * TableRollingFrictionCoefficient)
	{
		//set the physics state to rolling
		SetBoolPhysicsState(Ebps_Rolling);

		//return to prevent further execution
		return;
	}

	//set the physics state to stationary
	SetBoolPhysicsState(Ebps_Stationary);

}

void ABallActor::UpdatePhysicsVariables(const float DeltaTime)
{
	//check the physics state
	switch (PhysicsState)
	{
		case Ebps_Spinning:
		{
			//set the angular velocity variable
			SetBallAngularVelocity(FVector(0, 0, (AngularVelocity.Z - 5 * TableSpinningFrictionCoefficient * (GetWorld()->GetGravityZ() / 100) / 2) * DeltaTime));

			//break
			break;
		}

		case Ebps_Rolling:
		{
			//get the linear velocity magnitude
			const float VelocityMagnitute = GetBallVelocity().Size() - TableRollingFrictionCoefficient * (-GetWorld()->GetGravityZ() / 100) * DeltaTime;

			//set the velocity
			SetBallVelocity(GetBallVelocity().GetSafeNormal() * VelocityMagnitute);

			//set the angular velocity variable
			SetBallAngularVelocity(FVector(0, GetBallVelocity().X / SphereComponent->GetScaledSphereRadius(), AngularVelocity.Z - 5 * TableRollingFrictionCoefficient * (-GetWorld()->GetGravityZ() / 100) * DeltaTime / (2 * SphereComponent->GetScaledSphereRadius())));

			//break
			break;
		}

		case Ebps_Sliding:
		{
			//storage for the world gravityz
			const float WorldGravityZ = -GetWorld()->GetGravityZ() / 100;

			//set the velocity
			SetBallVelocity(GetBallVelocity() - TableSlidingFrictionCoefficient * WorldGravityZ * DeltaTime * InitialRelativeVelocity.GetSafeNormal());

			//update the initial relative velocity
			InitialRelativeVelocity = GetBallVelocity() - 2 / 7 * TableSlidingFrictionCoefficient * WorldGravityZ * DeltaTime * InitialRelativeVelocity.GetSafeNormal();

			//set the angular velocity variable
			SetBallAngularVelocity(FVector(
				AngularVelocity.X - 5 * TableSlidingFrictionCoefficient * WorldGravityZ * DeltaTime / (2 * SphereComponent->GetScaledSphereRadius()),
				AngularVelocity.Y - 5 * TableSlidingFrictionCoefficient * WorldGravityZ * DeltaTime / (2 * SphereComponent->GetScaledSphereRadius()),
				AngularVelocity.Z - 5 * TableSpinningFrictionCoefficient * WorldGravityZ * DeltaTime / (2 * SphereComponent->GetScaledSphereRadius())));

			//break
			break;
		}
		default: ;
	}
}

void ABallActor::SetBoolPhysicsState(const TEnumAsByte<EBallPhysicsState> NewPhysicsState)
{
	//check if there is not a change in physics state
	if (PhysicsState == NewPhysicsState)
	{
		//return early to prevent further execution
		return;
	}

	//set the new bool physics state
	PhysicsState = NewPhysicsState;
}

bool ABallActor::SolveStationaryBallMovingBallCollision(const FHitResult& Hit, const TObjectPtr<ABallActor> OtherBallActor)
{
	//get the angle between the velocities of the 2 balls
	const float Angle = FMath::Acos(FVector::DotProduct(GetBallVelocity().GetSafeNormal(), OtherBallActor->CurrentVelocity.GetSafeNormal()));

	//storage for the old velocity
	FVector OldVelocity = GetBallVelocity();

	//check if we have any old velocities
	if (OldVelocities.Num() > 0)
	{
		//set the old velocity to the last element in the array
		OldVelocity = OldVelocities.Last();
	}

	//get the direction from this ball to the other ball
	const FVector DirectionVec = (OtherBallActor->SphereComponent->GetComponentLocation() - SphereComponent->GetComponentLocation()).GetSafeNormal();

	//get our new velocity
	const FVector OurNewVel = -Hit.Normal * (OldVelocity * FMath::Cos(Angle)) * OldVelocity.GetSafeNormal();

	//get the other balls new velocity
	const FVector OtherBallNewVel = -Hit.Normal * (OldVelocity * FMath::Sin(Angle)) * DirectionVec * StationaryCollisionMultiplier;

	//check if we're in debug mode
	if (bDebugMode) 
	{
		//Draw a debug sphere at the location of the hit
		DrawDebugSphere(GetWorld(), Hit.Location, SphereComponent->GetScaledSphereRadius(), 16, FColor::Blue, false, 5);
	}

	//check if the new velocities are valid
	if (!FTransform(OurNewVel).IsValid() || ! FTransform(OtherBallNewVel).IsValid())
	{
		//return false
		return false;
	}

	//set the new velocities
	SetBallVelocity(OurNewVel);
	OtherBallActor->SetBallVelocity(OtherBallNewVel);

	//set the physics state to rolling for the other ball
	OtherBallActor->SetBoolPhysicsState(Ebps_Rolling);

	//return true
	return true;
}

bool ABallActor::ProcessBallHit(AActor* OtherActor, const FHitResult& Hit)
{
	//check if the other actor is not a ball actor
	if (!OtherActor->IsA(StaticClass()))
	{
		//return false
		return false;
	}

	//check if the other actor is a cue ball
	if (OtherActor->IsA(ACueBall::StaticClass()))
	{
		//get the cue ball
		const TObjectPtr<ACueBall> CueBall = Cast<ACueBall>(OtherActor);

		//loop through the ball upgrade data assets
		for (const TObjectPtr<UBallUpgradeDataAsset> BallUpgradeDataAsset : BallUpgradeDataAssets)
		{
			//call the OnCueBallHit function
			BallUpgradeDataAsset->OnCueBallHit(this, CueBall, Hit);
		}
	}
	else
	{
		//get the other ball actor
		const TObjectPtr<ABallActor> OtherBallActor = Cast<ABallActor>(OtherActor);

		//loop through the ball upgrade data assets
		for (const TObjectPtr<UBallUpgradeDataAsset> BallUpgradeDataAsset : BallUpgradeDataAssets)
		{
			//call the OnNormalBallHit function
			BallUpgradeDataAsset->OnNormalBallHit(this, OtherBallActor, Hit);
		}
	}

	//check if we're not using custom collision response
	if (!bUseCustomCollisionResponse)
	{
		//return early to prevent further execution
		return true;
	}

	//check if the other ball is stationary
	if (const TObjectPtr<ABallActor> OtherBallActor = Cast<ABallActor>(OtherActor); OtherBallActor->IsBallStationary())
	{
		//solve for stationary against moving ball collision
		SolveStationaryBallMovingBallCollision(Hit, OtherBallActor);
	}

	//return true
	return true;
}

void ABallActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//process the hit as if it was a ball hit
	ProcessBallHit(OtherActor, SweepResult);

	////create a debug circle
	//DrawDebugSphere(GetWorld(), SweepResult.Location, SphereComponent->GetScaledSphereRadius(), 16, FColor::Yellow, true, 5);

	////print an on screen debug message
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::Blue, OtherActor->GetActorNameOrLabel());
}

void ABallActor::OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//check if the other actor is a ball actor
	if (ProcessBallHit(OtherActor, Hit))
	{
		//return early to prevent further execution
		return;
	};

	//loop through the ball upgrade data assets
	for (UBallUpgradeDataAsset* BallUpgradeDataAsset : BallUpgradeDataAssets)
	{
		//call the OnWallHit function
		BallUpgradeDataAsset->OnWallHit(this, HitComponent, Hit);
	}
}

FVector ABallActor::GetBallVelocity() const
{
	//check if we're not using custom collision response
	if (!bUseCustomCollisionResponse)
	{
		//return the physics linear velocity
		return SphereComponent->GetPhysicsLinearVelocity();
	}

	//return the current velocity variable
	return CurrentVelocity;
}

FVector ABallActor::GetBallAngularVelocity() const
{
	//check if we're not using custom collision response
	if (!bUseCustomCollisionResponse)
	{
		//return the physics angular velocity
		return SphereComponent->GetPhysicsAngularVelocityInRadians();
	}

	//return the angular velocity variable
	return AngularVelocity;
}

bool ABallActor::IsBallStationary() const
{
	//check if we're not using custom collision response
	if (!bUseCustomCollisionResponse)
	{
		//return if the physics linear velocity is less than the stationary speed
		return SphereComponent->GetPhysicsLinearVelocity().Size() <= StationarySpeed;
	}

	//return if whether or not the bool physics state is stationary or spinning
	return PhysicsState == Ebps_Stationary || PhysicsState == Ebps_Spinning;

}

void ABallActor::SetBallVelocity(const FVector& NewVelocity)
{
	//check if we're not using custom collision response
	if (!bUseCustomCollisionResponse)
	{
		//set all physics linear velocity
		SphereComponent->SetAllPhysicsLinearVelocity(NewVelocity);
	}

	//set the current velocity variable
	CurrentVelocity = NewVelocity;
}

void ABallActor::SetBallAngularVelocity(const FVector& NewAngularVelocity)
{
	//check if we're not using custom collision response
	if (!bUseCustomCollisionResponse)
	{
		//set the physics angular velocity
		SphereComponent->SetAllPhysicsAngularVelocityInRadians(NewAngularVelocity);
	}

	//set the angular velocity variable
	AngularVelocity = NewAngularVelocity;
}

