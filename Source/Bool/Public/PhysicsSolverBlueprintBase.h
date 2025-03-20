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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector WallCollisionSetExitDirection(ABallActor* UnModifiedBall, const FHitResult& Hit) const;
	FVector WallCollisionSetExitDirection_Implementation(ABallActor* UnModifiedBall, const FHitResult& Hit) const;

	//sets the magnitude of the velocity of a ball after a collision with a wall
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float WallCollisionSetExitSpeed(ABallActor* UnModifiedBall, const FHitResult& Hit) const;
	float WallCollisionSetExitSpeed_Implementation(ABallActor* UnModifiedBall, const FHitResult& Hit) const;

	//sets the exit angular velocity direction of a ball after a collision with a wall
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FRotator WallCollisionSetAngularExitDirection(ABallActor* UnModifiedBall, const FHitResult& Hit) const;
	FRotator WallCollisionSetAngularExitDirection_Implementation(ABallActor* UnModifiedBall, const FHitResult& Hit) const;

	//sets the magnitude of the angular velocity of a ball after a collision with a wall
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float WallCollisionSetAngularExitSpeed(ABallActor* UnModifiedBall, const FHitResult& Hit) const;
	float WallCollisionSetAngularExitSpeed_Implementation(ABallActor* UnModifiedBall, const FHitResult& Hit) const;

	//sets the exit direction of a ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector BallCollisionSetExitDirection(ABallActor* UnModifiedBall, ABallActor* OtherBallActor, const FHitResult& Hit) const;
	FVector BallCollisionSetExitDirection_Implementation(ABallActor* UnModifiedBall, ABallActor* OtherBallActor, const FHitResult& Hit) const;

	//sets the magnitude of the velocity of a ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float BallCollisionSetExitSpeed(ABallActor* UnModifiedBall, ABallActor* OtherBallActor, FVector OutDirection, const FHitResult& Hit) const;
	float BallCollisionSetExitSpeed_Implementation(ABallActor* UnModifiedBall, ABallActor* OtherBallActor, FVector OutDirection, const FHitResult& Hit) const;

	//sets the angular exit velocity direction of a ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FRotator BallCollisionSetAngularExitDirection(ABallActor* UnModifiedBall, ABallActor* OtherBallActor, const FHitResult& Hit) const;
	FRotator BallCollisionSetAngularExitDirection_Implementation(ABallActor* UnModifiedBall, ABallActor* OtherBallActor, const FHitResult& Hit) const;

	//sets the magnitude of the angular velocity of a ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float BallCollisionSetAngularExitSpeed(ABallActor* UnModifiedBall, ABallActor* OtherBallActor, FVector OutDirection, const FHitResult& Hit) const;
	float BallCollisionSetAngularExitSpeed_Implementation(ABallActor* UnModifiedBall, ABallActor* OtherBallActor, FVector OutDirection, const FHitResult& Hit) const;

	//sets the exit direction of the other ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector OtherBallCollisionSetExitDirection(ABallActor* OtherBallActor, ABallActor* UnModifiedBall, const FHitResult& Hit) const;
	FVector OtherBallCollisionSetExitDirection_Implementation(ABallActor* OtherBallActor, ABallActor* UnModifiedBall, const FHitResult& Hit) const;

	//sets the magnitude of the velocity of the other ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float OtherBallCollisionSetExitSpeed(ABallActor* OtherBallActor, ABallActor* UnModifiedBall, FVector OutDirection, const FHitResult& Hit) const;
	float OtherBallCollisionSetExitSpeed_Implementation(ABallActor* OtherBallActor, ABallActor* UnModifiedBall, FVector OutDirection, const FHitResult& Hit) const;

	//sets the angular exit direction of the other ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FRotator OtherBallCollisionSetAngularExitDirection(ABallActor* OtherBallActor, ABallActor* UnModifiedBall, const FHitResult& Hit) const;
	FRotator OtherBallCollisionSetAngularExitDirection_Implementation(ABallActor* OtherBallActor, ABallActor* UnModifiedBall, const FHitResult& Hit) const;

	//sets the magnitude of the angular velocity of the other ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float OtherBallCollisionSetAngularExitSpeed(ABallActor* OtherBallActor, ABallActor* UnModifiedBall, FVector OutDirection, const FHitResult& Hit) const;
	float OtherBallCollisionSetAngularExitSpeed_Implementation(ABallActor* OtherBallActor, ABallActor* UnModifiedBall, FVector OutDirection, const FHitResult& Hit) const;
	
	//updates a balls velocity every tick
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateBallVelocity(ABallActor* BallActor, float DeltaTime) const;
	void UpdateBallVelocity_Implementation(ABallActor* BallActor, float DeltaTime) const;

	//updates a balls angular velocity every tick
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateBallAngularVelocity(ABallActor* BallActor, float DeltaTime) const;
	void UpdateBallAngularVelocity_Implementation(ABallActor* BallActor, float DeltaTime) const;

	//performs the movement for a ball actor every tick
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PerformMovement(ABallActor* BallActor, float DeltaTime) const;
	void PerformMovement_Implementation(ABallActor* BallActor, float DeltaTime) const;
};
