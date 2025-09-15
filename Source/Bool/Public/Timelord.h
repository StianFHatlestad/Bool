// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "gameframework/Actor.h"
#include "Balls/BallActor.h"
#include "Kismet/GameplayStatics.h"
#include "BoolGameInstance.h"

#include "Timelord.generated.h"

class UBoolGameInstance;


/**
 * 
 */
UCLASS()
class BOOL_API ATimelord : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:

	ATimelord();
	~ATimelord();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Rewinds the balls using position and rotation from the ballactors
	UFUNCTION(BlueprintCallable)
	void startRewind();

	//Reasurres that rewinding is possible on the ball
	UFUNCTION(BlueprintCallable)
	bool RewindCheck(ABallActor* Ball);

	//Creates a new entry for history data 
	UFUNCTION(BlueprintCallable)
	void CreateNewEntry();

	//Records the position and rotation data of a ball
	UFUNCTION(BlueprintCallable)
	void RecordPosAndRot();

	

	//Keeps track of the correct index for rewinding
	UPROPERTY(BlueprintReadOnly)
	int rewindIndex{ 0 };

	//Whether or not the timelord is currently rewinding
	bool bIsRecording = false;

	// Reference to the game instance
	UPROPERTY()
	TObjectPtr<UBoolGameInstance> GameInstance = nullptr;

};
