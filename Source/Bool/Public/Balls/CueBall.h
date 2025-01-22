// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BallActor.h"
#include "GameFramework/Actor.h"
#include "CueBall.generated.h"

UCLASS()
class BOOL_API ACueBall : public ABallActor
{
	GENERATED_BODY()
	
public:

	//the start position of the ball
	UPROPERTY(BlueprintReadOnly)
	FVector StartPosition = FVector::ZeroVector;

	//constructor(s)
	ACueBall();


	//override(s)
	virtual void BeginPlay() override;
};
