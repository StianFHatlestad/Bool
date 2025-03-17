


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

void UPhysicsSolverBlueprintBase::UpdateBallVelocity_Implementation(ABallActor* BallActor, float DeltaTime) const
{
	//check the physics state
	switch (BallActor->PhysicsState)
	{
		case Ebps_Spinning:
		{
			//set the angular velocity variable
			BallActor->SetBallAngularVelocity(FVector(0, 0, (BallActor->AngularVelocity.Z - 5 * BallActor->TableSpinningFrictionCoefficient * (GetWorld()->GetGravityZ() / 100) / 2) * DeltaTime));

			//break
			break;
		}

		case Ebps_Rolling:
		{
			//get the linear velocity magnitude
			const float VelocityMagnitute = BallActor->GetBallVelocity().Size() - BallActor->TableRollingFrictionCoefficient * (-GetWorld()->GetGravityZ() / 100) * DeltaTime;

			//set the velocity
			BallActor->SetBallVelocity(BallActor->GetBallVelocity().GetSafeNormal() * VelocityMagnitute);

			//set the angular velocity variable
			BallActor->SetBallAngularVelocity(FVector(0, BallActor->GetBallVelocity().X / BallActor->SphereComponent->GetScaledSphereRadius(), BallActor->AngularVelocity.Z - 5 * BallActor->TableRollingFrictionCoefficient * (-GetWorld()->GetGravityZ() / 100) * DeltaTime / (2 * BallActor->SphereComponent->GetScaledSphereRadius())));

			//break
			break;
		}

		case Ebps_Sliding:
		{
			//storage for the world gravityz
			const float WorldGravityZ = -GetWorld()->GetGravityZ() / 100;

			//set the velocity
			BallActor->SetBallVelocity(BallActor->GetBallVelocity() - BallActor->TableSlidingFrictionCoefficient * WorldGravityZ * DeltaTime * BallActor->InitialRelativeVelocity.GetSafeNormal());

			//update the initial relative velocity
			BallActor->InitialRelativeVelocity = BallActor->GetBallVelocity() - 2 / 7 * BallActor->TableSlidingFrictionCoefficient * WorldGravityZ * DeltaTime * BallActor->InitialRelativeVelocity.GetSafeNormal();

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

//FVector UPhysicsDataBlueprint::MulitBallCollisionExitDir_Implementation(ABallActor* UnModifiedBall, ABallActor* OtherBallActor, FVector IncomingDirection, FVector CollisionPoint, FVector HitNormal)
//{
//	//get the velocity of the first ball reflected off the other ball
//	FVector ReturnVec = UKismetMathLibrary::GetReflectionVector(IncomingDirection, -HitNormal);
//
//	//return 
//	return FVector(ReturnVec.X, ReturnVec.Y, 0).GetSafeNormal();
//}
//
//float UPhysicsDataBlueprint::MulitBallCollisionExitSpeed_Implementation(ABallActor* UnModifiedBall, ABallActor* OtherBallActor, FVector IncomingDirection, FVector CollisionPoint, FVector HitNormal, FVector ExitDirection, float Force)
//{
//	//return the force
//	return Force;
//}
