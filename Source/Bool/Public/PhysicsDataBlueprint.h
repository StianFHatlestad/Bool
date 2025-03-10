#pragma once
#include "CoreMinimal.h"
#include "PhysicsDataBlueprint.generated.h"

//forward declarations
class ABallActor;

/**
 * Class for getting physics data in a highly customizable way.
 */
UCLASS(Blueprintable)
class UPhysicsDataBlueprint : public UObject
{
	GENERATED_BODY()
public:

	//constructor(s)
	UPhysicsDataBlueprint();

	//whether or not to use seperate multi-body collision resolution
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData|Physics")
	bool bUseMultiBodyCollisionResolution = false;

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

	////sets the exit direction of a ball after a collision with another ball in a multi-body collision
	//UFUNCTION(BlueprintNativeEvent)
	//FVector MulitBallCollisionExitDir(ABallActor* UnModifiedBall, ABallActor* OtherBallActor, FVector IncomingDirection, FVector CollisionPoint, FVector HitNormal);
	//FVector MulitBallCollisionExitDir_Implementation(ABallActor* UnModifiedBall, ABallActor* OtherBallActor, FVector IncomingDirection, FVector CollisionPoint, FVector HitNormal);

	////sets the exit speed of a ball after a collision with another ball in a multi-body collision
	//UFUNCTION(BlueprintNativeEvent)
	//float MulitBallCollisionExitSpeed(ABallActor* UnModifiedBall, ABallActor* OtherBallActor, FVector IncomingDirection, FVector CollisionPoint, FVector HitNormal, FVector ExitDirection, float Force);
	//float MulitBallCollisionExitSpeed_Implementation(ABallActor* UnModifiedBall, ABallActor* OtherBallActor, FVector IncomingDirection, FVector CollisionPoint, FVector HitNormal, FVector ExitDirection, float Force);

};
