 // Fill out your copyright notice in the Description page of Project Settings.


#include "Timelord.h"


 


ATimelord::ATimelord()
{
	// Enable ticking every frame
	PrimaryActorTick.bCanEverTick = true;

	
}

ATimelord::~ATimelord()
{
}


void ATimelord::BeginPlay()
{
	//call the parent implementation
	Super::BeginPlay();

	//Get the game instance
	GameInstance = Cast<UBoolGameInstance>(UGameplayStatics::GetGameInstance(this));
}

void ATimelord::Tick(float DeltaTime)
{
	
	//call the parent implementation
	Super::Tick(DeltaTime);

	if (GameInstance->IsValidLowLevel())
	{
		if (GameInstance->bTurnInProgress)
		{ 
			//Start recording, new entry created in playerpawn when turn starts
			bIsRecording = true;
		}
		else
		{
			bIsRecording = false;
		}

		if (bIsRecording)
		{
			RecordPosAndRot();
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::White, "reeeeecording");
		}
	}
}

//TODO: implement this porperly in the UI
void ATimelord::startRewind()
{

	//maybe not needed
}

void ATimelord::CreateNewEntry()
{
	
	//Increase the rewindIndex
	//rewindIndex += 1;

	//Get all the balls on the scene
	TArray<AActor*> Balls;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABallActor::StaticClass(), Balls);

	for (AActor* BallActor : Balls)
	{
		//cast the ball actor to a ball actor
		const TObjectPtr<ABallActor> Ball = Cast<ABallActor>(BallActor);
		if (Ball->IsValidLowLevel())
		{
			//add a new position and rotation struct to the history
			Ball->PositionAndRotationHistory.Add(FPositionAndRotationData());

			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Green, "New entry added");
		}
	}
}

void ATimelord::RecordPosAndRot()
{
	/// TODO:remove
} 

void ATimelord::turnOnRewinding()
{
	bIsRewinding = true;
	
}

void ATimelord::rewinding()
{/*
	TArray<AActor*> Balls;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABallActor::StaticClass(), Balls);
	if (bIsRewinding)
	{
		for (AActor* BallActor : Balls)
		{
			//cast the ball actor to a ball actor
			const TObjectPtr<ABallActor> Ball = Cast<ABallActor>(BallActor);
			if (Ball->RewindToIndex(rewindIndex, posIndex))
			{
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::green, "Rewinding");
			}
		}
		bIsRewinding = false;
	}*/
}

bool ATimelord::RewindCheck(ABallActor* Ball)
{
	//Check if index is valid
	if (!Ball || !Ball->IsValidLowLevelFast())
	{
		return false; // If the ball is not valid, do nothing
	}

	//Check if a history entry exists for the current rewind index
	if (!Ball->PositionAndRotationHistory.IsValidIndex(rewindIndex))
	{
		
		return false;
	}

	//check if the ball has a position and rotation history
	if (Ball->PositionAndRotationHistory.IsEmpty())
	{
		//return early to prevent further execution
		return false;
	}

	//Check if the vector holding the positions is not empty
	if (Ball->PositionAndRotationHistory.Last().Positions.Num() == 0)
	{
		//return early to prevent further execution
		return false;
	}
	return true;
}