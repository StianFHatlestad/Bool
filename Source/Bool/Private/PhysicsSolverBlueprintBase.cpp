


#include "PhysicsSolverBlueprintBase.h"

#include "Balls/BallActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"


UPhysicsSolverBlueprintBase::UPhysicsSolverBlueprintBase()
{
}

FVector UPhysicsSolverBlueprintBase::WallCollisionSetExitDirection_Implementation( ABallActor* UnModifiedBall, const FHitResult& Hit) const
{
	return FVector::ZeroVector;
}

float UPhysicsSolverBlueprintBase::WallCollisionSetExitSpeed_Implementation(ABallActor* UnModifiedBall, const FHitResult& Hit) const
{
	return 1;
}

FRotator UPhysicsSolverBlueprintBase::WallCollisionSetAngularExitDirection_Implementation(ABallActor* UnModifiedBall, const FHitResult& Hit) const
{
	return FRotator::ZeroRotator;
}

float UPhysicsSolverBlueprintBase::WallCollisionSetAngularExitSpeed_Implementation(ABallActor* UnModifiedBall, const FHitResult& Hit) const
{
	return 1;
}

FVector UPhysicsSolverBlueprintBase::BallCollisionSetExitDirection_Implementation(ABallActor* UnModifiedBall, ABallActor* OtherBallActor, const FHitResult& Hit) const
{
	return FVector::ZeroVector;
}

float UPhysicsSolverBlueprintBase::BallCollisionSetExitSpeed_Implementation(ABallActor* UnModifiedBall, ABallActor* OtherBallActor, FVector OutDirection, const FHitResult& Hit) const
{
	return 1;
}

FRotator UPhysicsSolverBlueprintBase::BallCollisionSetAngularExitDirection_Implementation(ABallActor* UnModifiedBall, ABallActor* OtherBallActor, const FHitResult& Hit) const
{
	return FRotator::ZeroRotator;
}

float UPhysicsSolverBlueprintBase::BallCollisionSetAngularExitSpeed_Implementation(ABallActor* UnModifiedBall, ABallActor* OtherBallActor, FVector OutDirection, const FHitResult& Hit) const
{
	return 1;
}

FVector UPhysicsSolverBlueprintBase::OtherBallCollisionSetExitDirection_Implementation(ABallActor* OtherBallActor, ABallActor* UnModifiedBall, const FHitResult& Hit) const
{
	return FVector::ZeroVector;
}

float UPhysicsSolverBlueprintBase::OtherBallCollisionSetExitSpeed_Implementation(ABallActor* OtherBallActor, ABallActor* UnModifiedBall, FVector OutDirection, const FHitResult& Hit) const
{
	return 1;
}

FRotator UPhysicsSolverBlueprintBase::OtherBallCollisionSetAngularExitDirection_Implementation(ABallActor* OtherBallActor, ABallActor* UnModifiedBall, const FHitResult& Hit) const
{
	return FRotator::ZeroRotator;
}

float UPhysicsSolverBlueprintBase::OtherBallCollisionSetAngularExitSpeed_Implementation(ABallActor* OtherBallActor, ABallActor* UnModifiedBall, FVector OutDirection, const FHitResult& Hit) const
{
	return 1;
}

void UPhysicsSolverBlueprintBase::UpdateBallVelocity_Implementation(ABallActor* BallActor, float DeltaTime) const
{
	
}

void UPhysicsSolverBlueprintBase::UpdateBallAngularVelocity_Implementation(ABallActor* BallActor, float DeltaTime) const
{
	
}

void UPhysicsSolverBlueprintBase::PerformMovement_Implementation(ABallActor* BallActor, float DeltaTime) const
{
	//move the component
	BallActor->SphereComponent->MoveComponent(BallActor->GetBallVelocity() * DeltaTime, BallActor->SphereComponent->GetComponentRotation() + BallActor->GetBallAngularVelocity() * DeltaTime, true);
}
