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
			//Create new entry for rewind history
			CreateNewEntry();
		}
		while (GameInstance->bTurnInProgress)
		{
			RecordPosAndRot();
		}		
	}

	while (bIsRewinding)
	{
		startRewind();
	}
	/*
	if (bRecordRewindData)
	{
		//add the current position and rotation to the last position and rotation struct
		//The function already compares new entry to last entry, and does not accept duplicate
		PositionAndRotationHistory.Last().AddPositionAndRotation(GetActorLocation(), GetActorRotation());
	}

	//Rewinds the ball if the rewind flag is set
	if (bIsRewinding)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red, FString::Printf(TEXT("R-Index %d, HistoryNum: %d"), GameInstance->rewindIndex, PositionAndRotationHistory.Num()));
		if (RewindCheck())
		{
			FVector targetLocation = PositionAndRotationHistory[GameInstance->rewindIndex].popLastPos();
			SetActorLocation(targetLocation);
		}

	}*/
}

//TODO: implement this porperly in the UI
void ATimelord::startRewind()
{
	
	//Get all the balls on the scene
	TArray<AActor*> Balls;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABallActor::StaticClass(), Balls);

	for (AActor* BallActor : Balls)
	{
		//cast the ball actor to a ball actor
		const TObjectPtr<ABallActor> Ball = Cast<ABallActor>(BallActor);
		//check if the ball is valid
		if (Ball->IsValidLowLevel())
		{
			Ball->RewindToIndex(rewindIndex);
		}
	}
}

void ATimelord::CreateNewEntry()
{
	
	//Increase the rewindIndex
	rewindIndex += 1;

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
		}
	}
}

void ATimelord::RecordPosAndRot()
{
	TArray<AActor*> Balls;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABallActor::StaticClass(), Balls);

	for (AActor* BallActor : Balls)
	{
		//cast the ball actor to a ball actor
		const TObjectPtr<ABallActor> Ball = Cast<ABallActor>(BallActor);

		if (Ball->IsValidLowLevel())
		{
			//add the current position and rotation to the last position and rotation struct
			Ball->PositionAndRotationHistory.Last().AddPositionAndRotation(Ball->GetActorLocation(), Ball->GetActorRotation());
		}
	}
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
