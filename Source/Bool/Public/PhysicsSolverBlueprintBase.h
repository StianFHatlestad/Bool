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

	//sets the exit angular velocity direction of a ball after a collision with a wall
	UFUNCTION(BlueprintNativeEvent)
	FVector WallCollisionSetAngularExitDirection(FVector SuggestedAngularVel, ABallActor* UnModifiedBall, const FHitResult& Hit) const;
	FVector WallCollisionSetAngularExitDirection_Implementation(FVector SuggestedAngularVel, ABallActor* UnModifiedBall, const FHitResult& Hit) const;

	//sets the magnitude of the angular velocity of a ball after a collision with a wall
	UFUNCTION(BlueprintNativeEvent)
	float WallCollisionSetAngularExitSpeed(FVector SuggestedAngularVel, ABallActor* UnModifiedBall, const FHitResult& Hit) const;
	float WallCollisionSetAngularExitSpeed_Implementation(FVector SuggestedAngularVel, ABallActor* UnModifiedBall, const FHitResult& Hit) const;

	//sets the exit direction of a ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent)
	FVector BallCollisionSetExitDirection(FVector SuggestedVelocity, ABallActor* UnModifiedBall, ABallActor* OtherBallActor, const FHitResult& Hit) const;
	FVector BallCollisionSetExitDirection_Implementation(FVector SuggestedVelocity, ABallActor* UnModifiedBall, ABallActor* OtherBallActor, const FHitResult& Hit) const;

	//sets the magnitude of the velocity of a ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent)
	float BallCollisionSetExitSpeed(FVector SuggestedVelocity, ABallActor* UnModifiedBall, ABallActor* OtherBallActor, FVector OutDirection, const FHitResult& Hit) const;
	float BallCollisionSetExitSpeed_Implementation(FVector SuggestedVelocity, ABallActor* UnModifiedBall, ABallActor* OtherBallActor, FVector OutDirection, const FHitResult& Hit) const;

	//sets the angular exit velocity direction of a ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent)
	FVector BallCollisionSetAngularExitDirection(FVector SuggestedAngularVel, ABallActor* UnModifiedBall, ABallActor* OtherBallActor, const FHitResult& Hit) const;
	FVector BallCollisionSetAngularExitDirection_Implementation(FVector SuggestedAngularVel, ABallActor* UnModifiedBall, ABallActor* OtherBallActor, const FHitResult& Hit) const;

	//sets the magnitude of the angular velocity of a ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent)
	float BallCollisionSetAngularExitSpeed(FVector SuggestedAngularVel, ABallActor* UnModifiedBall, ABallActor* OtherBallActor, FVector OutDirection, const FHitResult& Hit) const;
	float BallCollisionSetAngularExitSpeed_Implementation(FVector SuggestedAngularVel, ABallActor* UnModifiedBall, ABallActor* OtherBallActor, FVector OutDirection, const FHitResult& Hit) const;

	//sets the exit direction of the other ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent)
	FVector OtherBallCollisionSetExitDirection(FVector OtherBallSuggestedVelocity, ABallActor* OtherBallActor, ABallActor* UnModifiedBall, const FHitResult& Hit) const;
	FVector OtherBallCollisionSetExitDirection_Implementation(FVector OtherBallSuggestedVelocity, ABallActor* OtherBallActor, ABallActor* UnModifiedBall, const FHitResult& Hit) const;

	//sets the magnitude of the velocity of the other ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent)
	float OtherBallCollisionSetExitSpeed(FVector OtherBallSuggestedVelocity, ABallActor* OtherBallActor, ABallActor* UnModifiedBall, FVector OutDirection, const FHitResult& Hit) const;
	float OtherBallCollisionSetExitSpeed_Implementation(FVector OtherBallSuggestedVelocity, ABallActor* OtherBallActor, ABallActor* UnModifiedBall, FVector OutDirection, const FHitResult& Hit) const;

	//sets the angular exit direction of the other ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent)
	FVector OtherBallCollisionSetAngularExitDirection(FVector SuggestedAngularVel, ABallActor* OtherBallActor, ABallActor* UnModifiedBall, const FHitResult& Hit) const;
	FVector OtherBallCollisionSetAngularExitDirection_Implementation(FVector SuggestedAngularVel, ABallActor* OtherBallActor, ABallActor* UnModifiedBall, const FHitResult& Hit) const;

	//sets the magnitude of the angular velocity of the other ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent)
	float OtherBallCollisionSetAngularExitSpeed(FVector SuggestedAngularVel, ABallActor* OtherBallActor, ABallActor* UnModifiedBall, FVector OutDirection, const FHitResult& Hit) const;
	float OtherBallCollisionSetAngularExitSpeed_Implementation(FVector SuggestedAngularVel, ABallActor* OtherBallActor, ABallActor* UnModifiedBall, FVector OutDirection, const FHitResult& Hit) const;

	//updates a balls velocity every tick
	UFUNCTION(BlueprintNativeEvent)
	void UpdateBallVelocity(ABallActor* BallActor, float DeltaTime) const;
	void UpdateBallVelocity_Implementation(ABallActor* BallActor, float DeltaTime) const;

	//updates a balls angular velocity every tick
	UFUNCTION(BlueprintNativeEvent)
	void UpdateBallAngularVelocity(ABallActor* BallActor, float DeltaTime) const;
	void UpdateBallAngularVelocity_Implementation(ABallActor* BallActor, float DeltaTime) const;
};
