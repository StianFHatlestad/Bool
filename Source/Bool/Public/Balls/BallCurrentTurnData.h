// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BallCurrentTurnData.generated.h"

/**
 * 
 */
UCLASS()
class BOOL_API UBallCurrentTurnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	//the amount of score the ball should give
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Balls|Default")
	int ScoreToGive = 200;

	//the amount of gold the ball should give
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Balls|Default")
	int GoldToGive = 15;
};
