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

FVector UPhysicsSolverBlueprintBase::BallCollisionSetExitDirection_Implementation(ABallActor* UnModifiedBall, TArray<ABallActor*>& OtherBalls, const FHitResult& Hit)
{
	return FVector::ZeroVector;
}

float UPhysicsSolverBlueprintBase::BallCollisionSetExitSpeed_Implementation(ABallActor* UnModifiedBall, TArray<ABallActor*>& OtherBalls, FVector OutDirection, const FHitResult& Hit)
{
	return 1;
}

FRotator UPhysicsSolverBlueprintBase::BallCollisionSetAngularExitDirection_Implementation(ABallActor* UnModifiedBall, TArray<ABallActor*>& OtherBalls, const FHitResult& Hit)
{
	return FRotator::ZeroRotator;
}

float UPhysicsSolverBlueprintBase::BallCollisionSetAngularExitSpeed_Implementation(ABallActor* UnModifiedBall, TArray<ABallActor*>& OtherBalls, FRotator OutDirection, const FHitResult& Hit)
{
	return 1;
}

FVector UPhysicsSolverBlueprintBase::OtherBallCollisionSetExitDirection_Implementation(TArray<ABallActor*>& OtherBalls, ABallActor* UnModifiedBall, ABallActor* InitialBall, const FHitResult& Hit)
{
	return FVector::ZeroVector;
}

float UPhysicsSolverBlueprintBase::OtherBallCollisionSetExitSpeed_Implementation(TArray<ABallActor*>& OtherBalls, ABallActor* UnModifiedBall, ABallActor* InitialBall, FVector OutDirection, const FHitResult& Hit)
{
	return 1;
}

FRotator UPhysicsSolverBlueprintBase::OtherBallCollisionSetAngularExitDirection_Implementation(TArray<ABallActor*>& OtherBalls, ABallActor* UnModifiedBall, ABallActor* InitialBall, const FHitResult& Hit)
{
	return FRotator::ZeroRotator;
}

float UPhysicsSolverBlueprintBase::OtherBallCollisionSetAngularExitSpeed_Implementation(TArray<ABallActor*>& OtherBalls, ABallActor* UnModifiedBall, ABallActor* InitialBall, FRotator OutDirection, const FHitResult& Hit)
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
	//move the component
	BallActor->SphereComponent->MoveComponent(BallActor->GetBallVelocity() * DeltaTime, BallActor->SphereComponent->GetComponentRotation() + BallActor->GetBallAngularVelocity() * DeltaTime, true);
}

void UPhysicsSolverBlueprintBase::ThisDrawDebugSphere(AActor* WorldContextObject, const FVector& Location, float Radius, int32 Segments, const FColor& Colour, bool PersistentLines, float LifeTime)
{
	DrawDebugSphere(WorldContextObject->GetWorld(), Location, Radius, Segments, Colour, PersistentLines, LifeTime);
}

void UPhysicsSolverBlueprintBase::ThisDrawDebugDirectionalArrow(AActor* WorldContextObject, const FVector& Start, const FVector& End, float Length, const FColor& Colour, bool PersistentLines, float LifeTime, int32 DepthPriority, float Thickness)
{
	DrawDebugDirectionalArrow(WorldContextObject->GetWorld(), Start, End, Length, Colour, PersistentLines, LifeTime, DepthPriority, Thickness);
}
