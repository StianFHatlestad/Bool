// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallActor.generated.h"

class UBallUpgradeDataAsset;

UENUM(BlueprintType)
enum EBallPhysicsState
{
	Ebps_Stationary,
	Ebps_Spinning,
	Ebps_Rolling,
	Ebps_Sliding,
};

//much of the physics is copied and modified from https://ekiefl.github.io/2020/04/24/pooltool-theory/

UCLASS()
class BOOL_API ABallActor : public AActor
{
	GENERATED_BODY()
	
public:

	//sphere component for the cue ball
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* SphereComponent;

	//mesh component for visibilty
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* MeshComponent;

	//whether or not to use debug mode
	UPROPERTY(EditAnywhere, Category = "BoolData|Debug")
	bool bDebugMode = false;

	//whether or not to display our physics state
	UPROPERTY(EditAnywhere, Category = "BoolData|Debug", meta = (EditCondition = "bDebugMode", EditConditionHides))
	bool bDisplayPhysicsState = false;

	//whether or not to draw arrows for the old and new velocities after a ball against ball collision
	UPROPERTY(EditAnywhere, Category = "BoolData|Debug", meta = (EditCondition = "bDebugMode", EditConditionHides))
	bool BallColDebugArrows = false;

	//whether or not to draw arrows for the old and new velocities after a ball against wall collision
	UPROPERTY(EditAnywhere, Category = "BoolData|Debug", meta = (EditCondition = "bDebugMode", EditConditionHides))
	bool WallColDebugArrows = false;

	//whether or not to use the unique colour scheme for our debug arrows
	UPROPERTY(EditAnywhere, Category = "BoolData|Debug", meta = (EditCondition = "bDebugMode", EditConditionHides))
	bool UniqueDebugArrows = false;

	//the current turn data for the ball
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BoolData|Turns")
	class UBallCurrentTurnData* CurrentTurnData;

	//the upgrade data asset(s) for the ball
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BoolData|Upgrade")
	TArray<TSubclassOf<UBallUpgradeDataAsset>> BallUpgradeDataAssets;

	//the angular velocity of the ball
	UPROPERTY(BlueprintReadOnly, Category = "BoolData|Physics")
	FVector AngularVelocity = FVector::ZeroVector;

	//the current physics state of the ball
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BoolData|Physics")
	TEnumAsByte<EBallPhysicsState> PhysicsState = Ebps_Stationary;

	//the spinning friction coefficient of the table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData|Physics|Spinning")
	float TableSpinningFrictionCoefficient = 0.6;

	//the rolling friction coefficient of the table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData|Physics|Rolling")
	float TableRollingFrictionCoefficient = 0.8;

	//the sliding friction coefficient of the table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData|Physics|Sliding")
	float TableSlidingFrictionCoefficient = 0.9;

	//the threshold for whether we're sliding or rolling (lowering leads to more being considered sliding instead of rolling)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData|Physics")
	float SlidingRollingThreshold = 50;

	//storage for the initial relative velocity between the ball and the table when starting to slide
	UPROPERTY(BlueprintReadOnly, Category = "BoolData|Physics|Sliding")
	FVector InitialRelativeVelocity = FVector::ZeroVector;

	//whether or not to use custom physics
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData|Physics")
	bool bUseCustomPhysics = true;

	//whether or not to use custom collision response (needs bSimulatePhysics to be true for the sphere component for this to work)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData|Physics")
	bool bUseCustomCollisionResponse = true;

	//the speed at which the ball is considered to be stationary
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData|Physics|Stationary")
	float StationarySpeed = 50;

	//the old velocities of the ball
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BoolData|Physics")
	TArray<FVector> OldVelocities = {FVector::ZeroVector};

	//our current velocity
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BoolData|Physics")
	FVector CurrentVelocity = FVector::ZeroVector;

	//the mass of the ball
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData|Physics")
	float BallMass = 10;

	//the actor tag to ignore when checking for collisions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData|Physics")
	FName ActorTagToIgnore = "IgnoreCol";

	//the restitution to use for ball to wall collisions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData|Physics|Collision")
	float WallRestitution = .2;

	//the friction to use for ball to wall collisions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData|Physics|Collision")
	float WallFriction = .85;

	//the restitution to use for ball to ball collisions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData|Physics|Collision")
	float BallRestitution = .999999;

	//the collision multiplier to use for ball to ball collisions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData|Physics|Collision")
	float BallBallCollisionMultiplier = .85;

	//the maximum relative speed gain from a collision between 2 balls (percentage)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData|Physics|Collision")
	float MaxRelativeSpeedGain = -1;

	//the first frictional constant for ball to ball collisions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData|Physics|Collision")
	float BallBallFrictionA = 9.951e-3;

	//the second frictional constant for ball to ball collisions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData|Physics|Collision")
	float BallBallFrictionB = 0.108;

	//the third frictional constant for ball to ball collisions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData|Physics|Collision")
	float BallBallFrictionC = 1.088;

	//the position of the box we use to check if we're outside the table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData|Physics")
	FVector BoxPosition = FVector::ZeroVector;

	//the size of the box we use to check if we're outside the table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData|Physics")
	FVector BoxSize = FVector(50, 50, 50);

	//constructor(s)
	ABallActor();

	//overrides(s)
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//function to update the old velocities
	void UpdateOldVelocities();

	//function to check if the state of the ball should be updated
	UFUNCTION(BlueprintCallable)
	void UpdateBoolPhysicsState(float DeltaTime);

	//function to update the physics variables of the ball
	UFUNCTION(BlueprintCallable)
	void UpdatePhysicsVariables(float DeltaTime);

	//function to set the bool physics state
	UFUNCTION(BlueprintCallable)
	void SetBoolPhysicsState(TEnumAsByte<EBallPhysicsState> NewPhysicsState);

	//function to check if we're outside the table
	UFUNCTION(BlueprintCallable)
	bool IsOutsideTable() const;

	//helper function to get the friction between 2 colliding balls
	float GetFrictionBetweenBalls(const TObjectPtr<ABallActor>& OtherBallActor) const;

	//debug function to draw arrows for the old and new velocities after a ball against ball collision
	void DrawVelChangeDebugArrows();

	//function to process a ball hit and call the appropriate event(s)
	bool ProcessBallHit(AActor* OtherActor, const FHitResult& Hit);

	//function to process a wall hit and call the appropriate event(s)
	void ProcessWallHit(const FHitResult& Hit);

	//function called when the ball overlaps with another actor
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//function called when the ball is hit
	UFUNCTION()
	void OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//function to get the velocity of the ball
	UFUNCTION(BlueprintCallable)
	FVector GetBallVelocity() const;

	//function to get the angular velocity of the ball
	UFUNCTION(BlueprintCallable)
	FVector GetBallAngularVelocity() const;

	//function to get the current friction coefficient we're using
	UFUNCTION(BlueprintCallable)
	float GetFrictionCoefficient() const;

	//function to check if we're stationary
	UFUNCTION(BlueprintCallable)
	bool IsBallStationary() const;

	//function to set the velocity of the ball
	UFUNCTION(BlueprintCallable)
	void SetBallVelocity(const FVector& NewVelocity);

	//function to set the angular velocity of the ball
	UFUNCTION(BlueprintCallable)
	void SetBallAngularVelocity(const FVector& NewAngularVelocity);

	//function to get a physics state enum value as a string
	FString GetPhysicsStateAsString(EBallPhysicsState InPhysicsState) const;
};
