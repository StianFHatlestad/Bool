


#include "PhysicsSolverBlueprintBase.h"

#include "Balls/BallActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"


UPhysicsSolverBlueprintBase::UPhysicsSolverBlueprintBase()
{
}

FVector UPhysicsSolverBlueprintBase::WallCollisionSetExitDirection_Implementation(FVector SuggestedVelocity, ABallActor* UnModifiedBall, const FHitResult& Hit) const
{
	//return the normalized suggested velocity
	return SuggestedVelocity.GetSafeNormal();
}

float UPhysicsSolverBlueprintBase::WallCollisionSetExitSpeed_Implementation(FVector SuggestedVelocity, ABallActor* UnModifiedBall, const FHitResult& Hit) const
{
	//return the size of the suggested velocity
	return SuggestedVelocity.Size();
}

FVector UPhysicsSolverBlueprintBase::WallCollisionSetAngularExitDirection_Implementation(FVector SuggestedAngularVel, ABallActor* UnModifiedBall, const FHitResult& Hit) const
{
	//return the suggested angular velocity normalized
	return SuggestedAngularVel.GetSafeNormal();
}

float UPhysicsSolverBlueprintBase::WallCollisionSetAngularExitSpeed_Implementation(FVector SuggestedAngularVel, ABallActor* UnModifiedBall, const FHitResult& Hit) const
{
	//return the length of the suggested angular velocity
	return SuggestedAngularVel.Length();
}

FVector UPhysicsSolverBlueprintBase::BallCollisionSetExitDirection_Implementation(FVector SuggestedVelocity, ABallActor* UnModifiedBall, ABallActor* OtherBallActor, const FHitResult& Hit) const
{
	//return the normalized suggested velocity
	return SuggestedVelocity.GetSafeNormal();
}

float UPhysicsSolverBlueprintBase::BallCollisionSetExitSpeed_Implementation(FVector SuggestedVelocity, ABallActor* UnModifiedBall, ABallActor* OtherBallActor, FVector OutDirection, const FHitResult& Hit) const
{
	//return the size of the suggested velocity
	return SuggestedVelocity.Size();
}

FVector UPhysicsSolverBlueprintBase::BallCollisionSetAngularExitDirection_Implementation(FVector SuggestedAngularVel, ABallActor* UnModifiedBall, ABallActor* OtherBallActor, const FHitResult& Hit) const
{
	//return the normalized suggested velocity
	return SuggestedAngularVel.GetSafeNormal();
}

float UPhysicsSolverBlueprintBase::BallCollisionSetAngularExitSpeed_Implementation(FVector SuggestedAngularVel, ABallActor* UnModifiedBall, ABallActor* OtherBallActor, FVector OutDirection, const FHitResult& Hit) const
{
	//return the length of the suggested angular velocity
	return SuggestedAngularVel.Length();
}

FVector UPhysicsSolverBlueprintBase::OtherBallCollisionSetExitDirection_Implementation(FVector OtherBallSuggestedVelocity, ABallActor* OtherBallActor, ABallActor* UnModifiedBall, const FHitResult& Hit) const
{
	//return the normalized suggested velocity
	return OtherBallSuggestedVelocity.GetSafeNormal();
}

float UPhysicsSolverBlueprintBase::OtherBallCollisionSetExitSpeed_Implementation(FVector OtherBallSuggestedVelocity, ABallActor* OtherBallActor, ABallActor* UnModifiedBall, FVector OutDirection, const FHitResult& Hit) const
{
	//return the size of the suggested velocity
	return OtherBallSuggestedVelocity.Size();
}

FVector UPhysicsSolverBlueprintBase::OtherBallCollisionSetAngularExitDirection_Implementation(FVector SuggestedAngularVel, ABallActor* OtherBallActor, ABallActor* UnModifiedBall, const FHitResult& Hit) const
{
	//return the normalized suggested velocity
	return SuggestedAngularVel.GetSafeNormal();
}

float UPhysicsSolverBlueprintBase::OtherBallCollisionSetAngularExitSpeed_Implementation(FVector SuggestedAngularVel, ABallActor* OtherBallActor, ABallActor* UnModifiedBall, FVector OutDirection, const FHitResult& Hit) const
{
	//return the length of the suggested angular velocity
	return SuggestedAngularVel.Length();
}

void UPhysicsSolverBlueprintBase::UpdateBallVelocity_Implementation(ABallActor* BallActor, float DeltaTime) const
{
	//check the physics state
	switch (BallActor->PhysicsState)
	{
		case Ebps_Rolling:
		{
			//get the linear velocity magnitude
			const float VelocityMagnitute = BallActor->GetBallVelocity().Size() - BallActor->TableRollingFrictionCoefficient * (-BallActor->GetWorld()->GetGravityZ() / 100) * DeltaTime;

			//set the velocity
			BallActor->SetBallVelocity(BallActor->GetBallVelocity().GetSafeNormal() * VelocityMagnitute);

			//break
			break;
		}

		case Ebps_Sliding:
		{
			//storage for the world gravity z
			const float WorldGravityZ = -BallActor->GetWorld()->GetGravityZ() / 100;

			//set the velocity
			BallActor->SetBallVelocity(BallActor->GetBallVelocity() - BallActor->TableSlidingFrictionCoefficient * WorldGravityZ * DeltaTime * BallActor->InitialRelativeVelocity.GetSafeNormal());

			//update the initial relative velocity
			BallActor->InitialRelativeVelocity = BallActor->GetBallVelocity() - 2 / 7 * BallActor->TableSlidingFrictionCoefficient * WorldGravityZ * DeltaTime * BallActor->InitialRelativeVelocity.GetSafeNormal();

			//break
			break;
		}
		default: ;
	}
}

void UPhysicsSolverBlueprintBase::UpdateBallAngularVelocity_Implementation(ABallActor* BallActor, float DeltaTime) const
{
	//check the physics state
	switch (BallActor->PhysicsState)
	{
		case Ebps_Spinning:
		{
			//set the angular velocity variable
			BallActor->SetBallAngularVelocity(FVector(0, 0, (BallActor->AngularVelocity.Z - 5 * BallActor->TableSpinningFrictionCoefficient * (BallActor->GetWorld()->GetGravityZ() / 100) / 2) * DeltaTime));

			//break
			break;
		}

		case Ebps_Rolling:
		{
			//set the angular velocity variable
			BallActor->SetBallAngularVelocity(FVector(0, BallActor->GetBallVelocity().X / BallActor->SphereComponent->GetScaledSphereRadius(), BallActor->AngularVelocity.Z - 5 * BallActor->TableRollingFrictionCoefficient * (-BallActor->GetWorld()->GetGravityZ() / 100) * DeltaTime / (2 * BallActor->SphereComponent->GetScaledSphereRadius())));

			//break
			break;
		}

		case Ebps_Sliding:
		{
			//storage for the world gravityz
			const float WorldGravityZ = -BallActor->GetWorld()->GetGravityZ() / 100;

			//set the angular velocity variable
			BallActor->SetBallAngularVelocity(FVector(
				BallActor->AngularVelocity.X - 5 * BallActor->TableSlidingFrictionCoefficient * WorldGravityZ * DeltaTime / (2 * BallActor->SphereComponent->GetScaledSphereRadius()),
				BallActor->AngularVelocity.Y - 5 * BallActor->TableSlidingFrictionCoefficient * WorldGravityZ * DeltaTime / (2 * BallActor->SphereComponent->GetScaledSphereRadius()),
				BallActor->AngularVelocity.Z - 5 * BallActor->TableSpinningFrictionCoefficient * WorldGravityZ * DeltaTime / (2 * BallActor->SphereComponent->GetScaledSphereRadius())));

			//break
			break;
		}
		default: ;
	}
}