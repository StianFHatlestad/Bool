// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallActor.generated.h"

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
	UStaticMeshComponent* MeshComponent;

	//the upgrade data asset(s) for the ball
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade")
	TArray<class UBallUpgradeDataAsset*> BallUpgradeDataAssets;

	//whether or not the ball is in the goal
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade")
	bool bInGoal = false;

	//the start position of the ball
	UPROPERTY(BlueprintReadOnly)
	FVector StartPosition;

	//constructor(s)
	ABallActor();

	//overrides(s)
	virtual void BeginPlay() override;

	//function called when the ball is hit
	UFUNCTION()
	void OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
