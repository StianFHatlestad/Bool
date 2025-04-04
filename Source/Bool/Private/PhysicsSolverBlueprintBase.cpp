#include "PhysicsSolverBlueprintBase.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"


APhysicsSolverBlueprintBase::APhysicsSolverBlueprintBase()
{
}

FVector APhysicsSolverBlueprintBase::WallCollisionSetExitDirection_Implementation( ABallActor* UnModifiedBall, const FHitResult& Hit)
{
	return FVector::ZeroVector;
}

float APhysicsSolverBlueprintBase::WallCollisionSetExitSpeed_Implementation(ABallActor* UnModifiedBall, const FHitResult& Hit)
{
	return 1;
}

FRotator APhysicsSolverBlueprintBase::WallCollisionSetAngularExitDirection_Implementation(ABallActor* UnModifiedBall, const FHitResult& Hit)
{
	return FRotator::ZeroRotator;
}

float APhysicsSolverBlueprintBase::WallCollisionSetAngularExitSpeed_Implementation(ABallActor* UnModifiedBall, const FHitResult& Hit)
{
	return 1;
}

FVector APhysicsSolverBlueprintBase::BallCollisionSetExitDirection_Implementation(ABallActor* UnModifiedBall, const TArray<ABallActor*>& OtherBalls, const FHitResult& Hit)
{
	return FVector::ZeroVector;
}

float APhysicsSolverBlueprintBase::BallCollisionSetExitSpeed_Implementation(ABallActor* UnModifiedBall, const TArray<ABallActor*>& OtherBalls, FVector OutDirection, const FHitResult& Hit)
{
	return 1;
}

FRotator APhysicsSolverBlueprintBase::BallCollisionSetAngularExitDirection_Implementation(ABallActor* UnModifiedBall, const TArray<ABallActor*>& OtherBalls, const FHitResult& Hit)
{
	return FRotator::ZeroRotator;
}

float APhysicsSolverBlueprintBase::BallCollisionSetAngularExitSpeed_Implementation(ABallActor* UnModifiedBall, const TArray<ABallActor*>& OtherBalls, FRotator OutDirection, const FHitResult& Hit)
{
	return 1;
}

FVector APhysicsSolverBlueprintBase::OtherBallCollisionSetExitDirection_Implementation(const TArray<ABallActor*>& OtherBalls, ABallActor* UnModifiedBall, ABallActor* InitialBall, FVector InitialBallOutDirection, const FHitResult& Hit)
{
	return FVector::ZeroVector;
}

float APhysicsSolverBlueprintBase::OtherBallCollisionSetExitSpeed_Implementation(const TArray<ABallActor*>& OtherBalls, ABallActor* UnModifiedBall, ABallActor* InitialBall, FVector OutDirection, const FHitResult& Hit)
{
	return 1;
}

FRotator APhysicsSolverBlueprintBase::OtherBallCollisionSetAngularExitDirection_Implementation(const TArray<ABallActor*>& OtherBalls, ABallActor* UnModifiedBall, ABallActor* InitialBall, const FHitResult& Hit)
{
	return FRotator::ZeroRotator;
}

float APhysicsSolverBlueprintBase::OtherBallCollisionSetAngularExitSpeed_Implementation(const TArray<ABallActor*>& OtherBalls, ABallActor* UnModifiedBall, ABallActor* InitialBall, FRotator OutDirection, const FHitResult& Hit)
{
	return 1;
}

void APhysicsSolverBlueprintBase::UpdateBallVelocity_Implementation(ABallActor* BallActor, float DeltaTime)
{
	
}

void APhysicsSolverBlueprintBase::UpdateBallAngularVelocity_Implementation(ABallActor* BallActor, float DeltaTime)
{
	
}

void APhysicsSolverBlueprintBase::PerformMovement_Implementation(ABallActor* BallActor, float DeltaTime)
{
	//rotation workaround to avoid gimbal lock from https://forums.unrealengine.com/t/how-can-i-rotate-a-pawn-a-full-360-degrees/281886/7
	FTransform orgRot(FRotationMatrix::MakeFromZX(BallActor->SphereComponent->GetUpVector(), BallActor->SphereComponent->GetForwardVector()).ToQuat());
    FQuat addInRot = (BallActor->GetBallAngularVelocity() * DeltaTime).Quaternion();
    orgRot.SetRotation(addInRot * orgRot.GetRotation());

	//move the component
	BallActor->SphereComponent->MoveComponent(BallActor->GetBallVelocity() * DeltaTime, orgRot.Rotator(), true);
}

void APhysicsSolverBlueprintBase::ThisDrawDebugSphere(AActor* WorldContextObject, const FVector& Location, float Radius, int32 Segments, const FColor& Colour, bool PersistentLines, float LifeTime)
{
	DrawDebugSphere(WorldContextObject->GetWorld(), Location, Radius, Segments, Colour, PersistentLines, LifeTime);
}

void APhysicsSolverBlueprintBase::ThisDrawDebugDirectionalArrow(AActor* WorldContextObject, const FVector& Start, const FVector& End, float Length, const FColor& Colour, bool PersistentLines, float LifeTime, int32 DepthPriority, float Thickness)
{
	DrawDebugDirectionalArrow(WorldContextObject->GetWorld(), Start, End, Length, Colour, PersistentLines, LifeTime, DepthPriority, Thickness);
}

void APhysicsSolverBlueprintBase::AddToBallRotation(ABallActor* InBall, FRotator InRot)
{
	//rotation workaround to avoid gimbal lock from https://forums.unrealengine.com/t/how-can-i-rotate-a-pawn-a-full-360-degrees/281886/7
	FTransform orgRot(FRotationMatrix::MakeFromZX(InBall->SphereComponent->GetUpVector(), InBall->SphereComponent->GetForwardVector()).ToQuat());
    orgRot.SetRotation(InRot.Quaternion() * orgRot.GetRotation());

	//set the rotation
	InBall->SphereComponent->SetWorldRotation(orgRot.Rotator());
}

void APhysicsSolverBlueprintBase::AddToBallAngularVelocity(ABallActor* InBall, FRotator InRot)
{
	//rotation workaround to avoid gimbal lock from https://forums.unrealengine.com/t/how-can-i-rotate-a-pawn-a-full-360-degrees/281886/7
	FTransform orgRot(FRotationMatrix::MakeFromZX(FVector::CrossProduct(InBall->AngularVelocity.Vector(), FVector(1,0,0)).GetSafeNormal(), InBall->AngularVelocity.Vector()).ToQuat());
    orgRot.SetRotation(InRot.Quaternion() * orgRot.GetRotation());

	//set the rotation
	InBall->AngularVelocity = orgRot.Rotator();
}
