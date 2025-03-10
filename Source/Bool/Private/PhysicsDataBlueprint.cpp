


#include "PhysicsDataBlueprint.h"

#include "Balls/BallActor.h"
#include "Kismet/KismetMathLibrary.h"


UPhysicsDataBlueprint::UPhysicsDataBlueprint()
{
}

FVector UPhysicsDataBlueprint::WallCollisionSetExitDirection_Implementation(FVector SuggestedVelocity, ABallActor* UnModifiedBall, const FHitResult& Hit) const
{
	//return the normalized suggested velocity
	return SuggestedVelocity.GetSafeNormal();
}

float UPhysicsDataBlueprint::WallCollisionSetExitSpeed_Implementation(FVector SuggestedVelocity, ABallActor* UnModifiedBall, const FHitResult& Hit) const
{
	//return the size of the suggested velocity
	return SuggestedVelocity.Size();
}

FVector UPhysicsDataBlueprint::BallCollisionSetExitDirection_Implementation(FVector SuggestedVelocity, ABallActor* UnModifiedBall, ABallActor* OtherBallActor, const FHitResult& Hit) const
{
	//return the normalized suggested velocity
	return SuggestedVelocity.GetSafeNormal();
}

float UPhysicsDataBlueprint::BallCollisionSetExitSpeed_Implementation(FVector SuggestedVelocity, ABallActor* UnModifiedBall, ABallActor* OtherBallActor, FVector OutDirection, const FHitResult& Hit) const
{
	//return the size of the suggested velocity
	return SuggestedVelocity.Size();
}

FVector UPhysicsDataBlueprint::OtherBallCollisionSetExitDirection_Implementation(FVector OtherBallSuggestedVelocity, ABallActor* OtherBallActor, ABallActor* UnModifiedBall, const FHitResult& Hit) const
{
	//return the normalized suggested velocity
	return OtherBallSuggestedVelocity.GetSafeNormal();
}

float UPhysicsDataBlueprint::OtherBallCollisionSetExitSpeed_Implementation(FVector OtherBallSuggestedVelocity, ABallActor* OtherBallActor, ABallActor* UnModifiedBall, FVector OutDirection, const FHitResult& Hit) const
{
	//return the size of the suggested velocity
	return OtherBallSuggestedVelocity.Size();
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
