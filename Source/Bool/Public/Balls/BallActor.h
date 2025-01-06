// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallActor.generated.h"

UENUM()
enum EBallPhysicsState
{
	Ebps_Stationary,
	Ebps_Spinning,
	Ebps_Rolling,
	Ebps_Sliding,
};

UCLASS()
class BOOL_API ABallActor : public AActor
{
	GENERATED_BODY()
	
public:

	//sphere component for the cue ball
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* SphereComponent;

	//mesh component for visibilty
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComponent;

	//the current turn data for the ball
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turns")
	class UBallCurrentTurnData* CurrentTurnData;

	//the upgrade data asset(s) for the ball
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade")
	TArray<class UBallUpgradeDataAsset*> BallUpgradeDataAssets;

	//the start position of the ball
	UPROPERTY(BlueprintReadOnly)
	FVector StartPosition = FVector::ZeroVector;

	//the displacement of the ball this frame
	UPROPERTY(BlueprintReadOnly, Category="Physics")
	FVector Displacement = FVector::ZeroVector;

	//the current physics state of the ball
	UPROPERTY(BlueprintReadOnly, Category = "Physics")
	TEnumAsByte<EBallPhysicsState> PhysicsState = Ebps_Stationary;

	//constructor(s)
	ABallActor();

	//overrides(s)
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//function to check if the state of the ball should be updated
	UFUNCTION(BlueprintCallable)
	void UpdateBoolPhysicsState(float DeltaTime);


	//function called when the ball is hit
	UFUNCTION()
	void OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
