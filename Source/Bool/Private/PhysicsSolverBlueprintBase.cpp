#include "PhysicsSolverBlueprintBase.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"


UPhysicsSolverBlueprintBase::UPhysicsSolverBlueprintBase()
{
}

FVector UPhysicsSolverBlueprintBase::WallCollisionSetExitDirection_Implementation( ABallActor* UnModifiedBall, const FHitResult& Hit)
{
	return FVector::ZeroVector;
}

float UPhysicsSolverBlueprintBase::WallCollisionSetExitSpeed_Implementation(ABallActor* UnModifiedBall, const FHitResult& Hit)
{
	return 1;
}

FRotator UPhysicsSolverBlueprintBase::WallCollisionSetAngularExitDirection_Implementation(ABallActor* UnModifiedBall, const FHitResult& Hit)
{
	return FRotator::ZeroRotator;
}

float UPhysicsSolverBlueprintBase::WallCollisionSetAngularExitSpeed_Implementation(ABallActor* UnModifiedBall, const FHitResult& Hit)
{
	return 1;
}

FVector UPhysicsSolverBlueprintBase::BallCollisionSetExitDirection_Implementation(ABallActor* UnModifiedBall, const TArray<ABallActor*>& OtherBalls, const FHitResult& Hit)
{
	return FVector::ZeroVector;
}

float UPhysicsSolverBlueprintBase::BallCollisionSetExitSpeed_Implementation(ABallActor* UnModifiedBall, const TArray<ABallActor*>& OtherBalls, FVector OutDirection, const FHitResult& Hit)
{
	return 1;
}

FRotator UPhysicsSolverBlueprintBase::BallCollisionSetAngularExitDirection_Implementation(ABallActor* UnModifiedBall, const TArray<ABallActor*>& OtherBalls, const FHitResult& Hit)
{
	return FRotator::ZeroRotator;
}

float UPhysicsSolverBlueprintBase::BallCollisionSetAngularExitSpeed_Implementation(ABallActor* UnModifiedBall, const TArray<ABallActor*>& OtherBalls, FRotator OutDirection, const FHitResult& Hit)
{
	return 1;
}

FVector UPhysicsSolverBlueprintBase::OtherBallCollisionSetExitDirection_Implementation(const TArray<ABallActor*>& OtherBalls, ABallActor* UnModifiedBall, ABallActor* InitialBall, FVector InitialBallOutDirection, const FHitResult& Hit)
{
	return FVector::ZeroVector;
}

float UPhysicsSolverBlueprintBase::OtherBallCollisionSetExitSpeed_Implementation(const TArray<ABallActor*>& OtherBalls, ABallActor* UnModifiedBall, ABallActor* InitialBall, FVector OutDirection, const FHitResult& Hit)
{
	return 1;
}

FRotator UPhysicsSolverBlueprintBase::OtherBallCollisionSetAngularExitDirection_Implementation(const TArray<ABallActor*>& OtherBalls, ABallActor* UnModifiedBall, ABallActor* InitialBall, const FHitResult& Hit)
{
	return FRotator::ZeroRotator;
}

float UPhysicsSolverBlueprintBase::OtherBallCollisionSetAngularExitSpeed_Implementation(const TArray<ABallActor*>& OtherBalls, ABallActor* UnModifiedBall, ABallActor* InitialBall, FRotator OutDirection, const FHitResult& Hit)
{
	return 1;
}

void UPhysicsSolverBlueprintBase::UpdateBallVelocity_Implementation(ABallActor* BallActor, float DeltaTime)
{
	
}

void UPhysicsSolverBlueprintBase::UpdateBallAngularVelocity_Implementation(ABallActor* BallActor, float DeltaTime)
{
	
}

void UPhysicsSolverBlueprintBase::PerformMovement_Implementation(ABallActor* BallActor, float DeltaTime)
{
	//rotation workaround to avoid gimbal lock from https://forums.unrealengine.com/t/how-can-i-rotate-a-pawn-a-full-360-degrees/281886/7
	FTransform orgRot(FRotationMatrix::MakeFromZX(BallActor->SphereComponent->GetUpVector(), BallActor->SphereComponent->GetForwardVector()).ToQuat());
    FQuat addInRot = (BallActor->GetBallAngularVelocity() * DeltaTime).Quaternion();
    orgRot.SetRotation(addInRot * orgRot.GetRotation());

	//move the component
	BallActor->SphereComponent->MoveComponent(BallActor->GetBallVelocity() * DeltaTime, orgRot.Rotator(), true);
}

void UPhysicsSolverBlueprintBase::ThisDrawDebugSphere(AActor* WorldContextObject, const FVector& Location, float Radius, int32 Segments, const FColor& Colour, bool PersistentLines, float LifeTime)
{
	DrawDebugSphere(WorldContextObject->GetWorld(), Location, Radius, Segments, Colour, PersistentLines, LifeTime);
}

void UPhysicsSolverBlueprintBase::ThisDrawDebugDirectionalArrow(AActor* WorldContextObject, const FVector& Start, const FVector& End, float Length, const FColor& Colour, bool PersistentLines, float LifeTime, int32 DepthPriority, float Thickness)
{
	DrawDebugDirectionalArrow(WorldContextObject->GetWorld(), Start, End, Length, Colour, PersistentLines, LifeTime, DepthPriority, Thickness);
}

void UPhysicsSolverBlueprintBase::AddToBallRotation(ABallActor* InBall, FRotator InRot)
{
	//rotation workaround to avoid gimbal lock from https://forums.unrealengine.com/t/how-can-i-rotate-a-pawn-a-full-360-degrees/281886/7
	FTransform orgRot(FRotationMatrix::MakeFromZX(InBall->SphereComponent->GetUpVector(), InBall->SphereComponent->GetForwardVector()).ToQuat());
    orgRot.SetRotation(InRot.Quaternion() * orgRot.GetRotation());

	//set the rotation
	InBall->SphereComponent->SetWorldRotation(orgRot.Rotator());
}

void UPhysicsSolverBlueprintBase::AddToBallAngularVelocity(ABallActor* InBall, FRotator InRot)
{
	//rotation workaround to avoid gimbal lock from https://forums.unrealengine.com/t/how-can-i-rotate-a-pawn-a-full-360-degrees/281886/7
	FTransform orgRot(FRotationMatrix::MakeFromZX(FVector::CrossProduct(InBall->AngularVelocity.Vector(), FVector(1,0,0)).GetSafeNormal(), InBall->AngularVelocity.Vector()).ToQuat());
    orgRot.SetRotation(InRot.Quaternion() * orgRot.GetRotation());

	//set the rotation
	InBall->AngularVelocity = orgRot.Rotator();
}

FRotator UPhysicsSolverBlueprintBase::RotationHelper(ABallActor* InBall, const FVector LastBallLocation, UCurveFloat* XCurve, UCurveFloat* YCurve) const
{
	//calculate the real velocity
	FVector RealVelocity = InBall->GetActorLocation() - LastBallLocation;

	//update the real velocity
	RealVelocity /= InBall->SphereComponent->GetScaledSphereRadius();
	RealVelocity *= -1;

	//storage for the pitch
	float Pitch = FMath::Atan(RealVelocity.X) * XCurve->GetFloatValue(RealVelocity.X) * FVector::DotProduct(-RealVelocity.GetSafeNormal(), FVector::XAxisVector);

	//storage for the roll
	float Roll = FMath::Atan(RealVelocity.Y) * YCurve->GetFloatValue(RealVelocity.Y) * FVector::DotProduct(-RealVelocity.GetSafeNormal(), FVector::YAxisVector);

	//storage for the yaw
	float TotalPitch = 
		FVector::DotProduct(RealVelocity.GetSafeNormal(), FVector::XAxisVector) * FMath::Atan(RealVelocity.X) * XCurve->GetFloatValue(RealVelocity.X) * FMath::Sign(FVector::DotProduct(RealVelocity.GetSafeNormal(), FVector::XAxisVector))
		+ FVector::DotProduct(RealVelocity.GetSafeNormal(), FVector::YAxisVector) * FMath::Atan(RealVelocity.Y) * YCurve->GetFloatValue(RealVelocity.Y) * FMath::Sign(FVector::DotProduct(RealVelocity.GetSafeNormal(), FVector::YAxisVector));

	return FRotator(Pitch * InBall->GetBallVelocity().Length(), TotalPitch * InBall->GetBallVelocity().Length(), Roll * InBall->GetBallVelocity().Length());
}

TArray<ABallActor*> UPhysicsSolverBlueprintBase::GetAllBallsInCluster(const TArray<ABallActor*>& Balls, ABallActor* BallToCheck, TArray<ABallActor*>& BallsToIgnore, int32 Depth, int32 MaxDepth) const
{
	//check if the depth is greater than the max depth
	if (Depth > MaxDepth)
	{
		//return empty array
		return TArray<ABallActor*>();
	}

	//temporary storage for the balls in the cluster
	TArray<ABallActor*> BallsInCluster = {};

	//check if the ball to check is not valid
	if (!BallToCheck->IsValidLowLevel())
	{
		//return empty array
		return BallsInCluster;
	}

	//check if the ball to check is not in the ignore list
	if (BallsToIgnore.Contains(BallToCheck))
	{
		//return empty array
		return BallsInCluster;
	}

	//add the ball to the cluster
	BallsInCluster.Add(BallToCheck);

	//add the ball to the ignore list
	BallsToIgnore.Add(BallToCheck);

	//loop through all the balls
	for (ABallActor* Ball : Balls)
	{
		//check if the ball is not the ball to check and is not in the ignore list
		if (Ball != BallToCheck && !BallsToIgnore.Contains(Ball))
		{
			//add the ball to the list of balls in the cluster
			BallsInCluster.Append(GetAllBallsInCluster(Balls, Ball, BallsToIgnore, Depth + 1, MaxDepth));
		}
	}
	return BallsInCluster;
}
