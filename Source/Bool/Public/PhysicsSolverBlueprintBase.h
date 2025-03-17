#pragma once
#include "CoreMinimal.h"
#include "PhysicsSolverBlueprintBase.generated.h"

//forward declarations
class ABallActor;

/**
 * Class for getting physics data in a highly customizable way.
 */
UCLASS(Blueprintable)
class UPhysicsSolverBlueprintBase : public UObject
{
	GENERATED_BODY()
public:

	//constructor(s)
	UPhysicsSolverBlueprintBase();

	//sets the exit direction of a ball after a collision with a wall
	UFUNCTION(BlueprintNativeEvent)
	FVector WallCollisionSetExitDirection(FVector SuggestedVelocity, ABallActor* UnModifiedBall, const FHitResult& Hit) const;
	FVector WallCollisionSetExitDirection_Implementation(FVector SuggestedVelocity, ABallActor* UnModifiedBall, const FHitResult& Hit) const;

	//sets the magnitude of the velocity of a ball after a collision with a wall
	UFUNCTION(BlueprintNativeEvent)
	float WallCollisionSetExitSpeed(FVector SuggestedVelocity, ABallActor* UnModifiedBall, const FHitResult& Hit) const;
	float WallCollisionSetExitSpeed_Implementation(FVector SuggestedVelocity, ABallActor* UnModifiedBall, const FHitResult& Hit) const;

	//sets the exit direction of a ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent)
	FVector BallCollisionSetExitDirection(FVector SuggestedVelocity, ABallActor* UnModifiedBall, ABallActor* OtherBallActor, const FHitResult& Hit) const;
	FVector BallCollisionSetExitDirection_Implementation(FVector SuggestedVelocity, ABallActor* UnModifiedBall, ABallActor* OtherBallActor, const FHitResult& Hit) const;

	//sets the magnitude of the velocity of a ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent)
	float BallCollisionSetExitSpeed(FVector SuggestedVelocity, ABallActor* UnModifiedBall, ABallActor* OtherBallActor, FVector OutDirection, const FHitResult& Hit) const;
	float BallCollisionSetExitSpeed_Implementation(FVector SuggestedVelocity, ABallActor* UnModifiedBall, ABallActor* OtherBallActor, FVector OutDirection, const FHitResult& Hit) const;

	//sets the exit direction of the other ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent)
	FVector OtherBallCollisionSetExitDirection(FVector OtherBallSuggestedVelocity, ABallActor* OtherBallActor, ABallActor* UnModifiedBall, const FHitResult& Hit) const;
	FVector OtherBallCollisionSetExitDirection_Implementation(FVector OtherBallSuggestedVelocity, ABallActor* OtherBallActor, ABallActor* UnModifiedBall, const FHitResult& Hit) const;

	//sets the magnitude of the velocity of the other ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent)
	float OtherBallCollisionSetExitSpeed(FVector OtherBallSuggestedVelocity, ABallActor* OtherBallActor, ABallActor* UnModifiedBall, FVector OutDirection, const FHitResult& Hit) const;
	float OtherBallCollisionSetExitSpeed_Implementation(FVector OtherBallSuggestedVelocity, ABallActor* OtherBallActor, ABallActor* UnModifiedBall, FVector OutDirection, const FHitResult& Hit) const;

	//updates a balls velocity every tick
	UFUNCTION(BlueprintNativeEvent)
	void UpdateBallVelocity(ABallActor* BallActor, float DeltaTime) const;
	void UpdateBallVelocity_Implementation(ABallActor* BallActor, float DeltaTime) const;

};
