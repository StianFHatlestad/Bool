// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BallUpgradeDataAsset.generated.h"

class AGoalActor;
class ABallActor;
/**
 * Data asset for a handling ball upgrades
 */
UCLASS()
class BOOL_API UBallUpgradeDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	//the name of the upgrade
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade")
	FName UpgradeName;

	//the description of the upgrade
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade")
	FText UpgradeDescription;

	//the image to use for the upgrade
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade")
	UTexture2D* UpgradeImage;

	//the level of this upgrade
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade")
	int32 UpgradeLevel = 0;

	//function for when the ball hits a wall
	UFUNCTION(BlueprintImplementableEvent)
	void OnWallHit(ABallActor* BallActor, UPrimitiveComponent* HitComponent, const FHitResult& Hit);

	//function for when the ball is hit by another ball(not the cue ball)
	UFUNCTION(BlueprintImplementableEvent)
	void OnNormalBallHit(ABallActor* BallActor, ABallActor* OtherActor, const FHitResult& Hit);

	//function for when the ball is hit by the cue ball
	UFUNCTION(BlueprintImplementableEvent)
	void OnCueBallHit(ABallActor* BallActor, ABallActor* CueBall, const FHitResult& Hit);

	//function for when the goes into a hole
	UFUNCTION(BlueprintImplementableEvent)
	void OnGoal(ABallActor* BallActor, AGoalActor* GoalActor);
};
