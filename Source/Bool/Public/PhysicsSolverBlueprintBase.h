#pragma once
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Balls/BallActor.h"
#include "PhysicsSolverBlueprintBase.generated.h"

/**
 * Class for getting physics data in a highly customizable way.
 */
UCLASS(Blueprintable)
class APhysicsSolverBlueprintBase : public AActor
{
	GENERATED_BODY()
public:

	//the amount of wiggle room for directionality when filtering collisions
	UPROPERTY(EditAnywhere)
	float BallColDetectionDirThreshold = -0.3;

	//constructor(s)
	APhysicsSolverBlueprintBase();

	//sets the exit direction of a ball after a collision with a wall
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector WallCollisionSetExitDirection(ABallActor* UnModifiedBall, const FHitResult& Hit);
	FVector WallCollisionSetExitDirection_Implementation(ABallActor* UnModifiedBall, const FHitResult& Hit);

	//sets the magnitude of the velocity of a ball after a collision with a wall
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float WallCollisionSetExitSpeed(ABallActor* UnModifiedBall, const FHitResult& Hit);
	float WallCollisionSetExitSpeed_Implementation(ABallActor* UnModifiedBall, const FHitResult& Hit);

	//sets the exit angular velocity direction of a ball after a collision with a wall
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FRotator WallCollisionSetAngularExitDirection(ABallActor* UnModifiedBall, const FHitResult& Hit);
	FRotator WallCollisionSetAngularExitDirection_Implementation(ABallActor* UnModifiedBall, const FHitResult& Hit);

	//sets the magnitude of the angular velocity of a ball after a collision with a wall
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float WallCollisionSetAngularExitSpeed(ABallActor* UnModifiedBall, const FHitResult& Hit);
	float WallCollisionSetAngularExitSpeed_Implementation(ABallActor* UnModifiedBall, const FHitResult& Hit);

	//sets the exit direction of a ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector BallCollisionSetExitDirection(ABallActor* UnModifiedBall, const TArray<ABallActor*>& OtherBalls, const FHitResult& Hit);
	FVector BallCollisionSetExitDirection_Implementation(ABallActor* UnModifiedBall, const TArray<ABallActor*>& OtherBalls, const FHitResult& Hit);

	//sets the magnitude of the velocity of a ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float BallCollisionSetExitSpeed(ABallActor* UnModifiedBall, const TArray<ABallActor*>& OtherBalls, FVector OutDirection, const FHitResult& Hit);
	float BallCollisionSetExitSpeed_Implementation(ABallActor* UnModifiedBall, const TArray<ABallActor*>& OtherBalls, FVector OutDirection, const FHitResult& Hit);

	//sets the angular exit velocity direction of a ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FRotator BallCollisionSetAngularExitDirection(ABallActor* UnModifiedBall, const TArray<ABallActor*>& OtherBalls, const FHitResult& Hit);
	FRotator BallCollisionSetAngularExitDirection_Implementation(ABallActor* UnModifiedBall, const TArray<ABallActor*>& OtherBalls, const FHitResult& Hit);

	//sets the magnitude of the angular velocity of a ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float BallCollisionSetAngularExitSpeed(ABallActor* UnModifiedBall, const TArray<ABallActor*>& OtherBalls, FRotator OutDirection, const FHitResult& Hit);
	float BallCollisionSetAngularExitSpeed_Implementation(ABallActor* UnModifiedBall, const TArray<ABallActor*>& OtherBalls, FRotator OutDirection, const FHitResult& Hit);

	//sets the exit direction of the other ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector OtherBallCollisionSetExitDirection(const TArray<ABallActor*>& OtherBalls, ABallActor* UnModifiedBall, ABallActor* InitialBall, FVector InitialBallOutDirection, const FHitResult& Hit);
	FVector OtherBallCollisionSetExitDirection_Implementation(const TArray<ABallActor*>& OtherBalls, ABallActor* UnModifiedBall, ABallActor* InitialBall, FVector InitialBallOutDirection, const FHitResult& Hit);

	//sets the magnitude of the velocity of the other ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float OtherBallCollisionSetExitSpeed(const TArray<ABallActor*>& OtherBalls, ABallActor* UnModifiedBall, ABallActor* InitialBall, FVector OutDirection, const FHitResult& Hit);
	float OtherBallCollisionSetExitSpeed_Implementation(const TArray<ABallActor*>& OtherBalls, ABallActor* UnModifiedBall, ABallActor* InitialBall, FVector OutDirection, const FHitResult& Hit);

	//sets the angular exit direction of the other ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FRotator OtherBallCollisionSetAngularExitDirection(const TArray<ABallActor*>& OtherBalls, ABallActor* UnModifiedBall, ABallActor* InitialBall, const FHitResult& Hit);
	FRotator OtherBallCollisionSetAngularExitDirection_Implementation(const TArray<ABallActor*>& OtherBalls, ABallActor* UnModifiedBall, ABallActor* InitialBall, const FHitResult& Hit);

	//sets the magnitude of the angular velocity of the other ball after a collision with another ball
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float OtherBallCollisionSetAngularExitSpeed(const TArray<ABallActor*>& OtherBalls, ABallActor* UnModifiedBall, ABallActor* InitialBall, FRotator OutDirection, const FHitResult& Hit);
	float OtherBallCollisionSetAngularExitSpeed_Implementation(const TArray<ABallActor*>& OtherBalls, ABallActor* UnModifiedBall, ABallActor* InitialBall, FRotator OutDirection, const FHitResult& Hit);
	
	//updates a balls velocity every tick
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateBallVelocity(ABallActor* BallActor, float DeltaTime);
	void UpdateBallVelocity_Implementation(ABallActor* BallActor, float DeltaTime);

	//updates a balls angular velocity every tick
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateBallAngularVelocity(ABallActor* BallActor, float DeltaTime);
	void UpdateBallAngularVelocity_Implementation(ABallActor* BallActor, float DeltaTime);

	//performs the movement for a ball actor every tick
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PerformMovement(ABallActor* BallActor, float DeltaTime);
	void PerformMovement_Implementation(ABallActor* BallActor, float DeltaTime);

	//function to draw a debug sphere
	UFUNCTION(BlueprintCallable, DisplayName = "Draw Debug Sphere")
	void ThisDrawDebugSphere(AActor* WorldContextObject, const FVector& Location, float Radius, int32 Segments, const FColor& Colour, bool PersistentLines, float LifeTime);

	//function to draw a debug arrow
	UFUNCTION(BlueprintCallable, DisplayName = "Draw Debug Directional Arrow")
	void ThisDrawDebugDirectionalArrow(AActor* WorldContextObject, const FVector& Start, const FVector& End, float Length, const FColor& Colour, bool PersistentLines, float LifeTime, int32 DepthPriority, float Thickness);
	
	//function to add to a balls rotation
	UFUNCTION(BlueprintCallable)
	void AddToBallRotation(ABallActor* InBall, FRotator InRot);

	//function to add to a balls angular velocity
	UFUNCTION(BlueprintCallable)
	void AddToBallAngularVelocity(ABallActor* InBall, FRotator InRot);
};
