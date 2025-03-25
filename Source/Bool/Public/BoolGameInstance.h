#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BoolGameInstance.generated.h"

//forward declaration(s)
class ABallActor;
class AGoalActor;

UCLASS()
class UBoolGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	//the target score for this round
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BoolData|Turns/Rounds")
	int DesiredRoundScore = 100;

	//the default amount of turns the player has each round
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BoolData|Turns/Rounds")
	int TurnsPerRound = 5;

	//the minimum time a turn must last (in seconds)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BoolData|Turns/Rounds")
	float MinTurnTime = .5;

	//the current amount of score the player has
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BoolData|Score")
	int PlayerScore = 0;

	//the current amount of gold the player has
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoolData|Gold")
	int PlayerGold = 0;

	//the next time a turn can be performed
	UPROPERTY(BlueprintReadOnly, Category="BoolData|Turns/Rounds")
	float AvailableTurnTime = 0;

	//the turn that the current round started on
	UPROPERTY(BlueprintReadOnly, Category = "BoolData|Turns/Rounds")
	int RoundStartTurn = 1;

	//the current turn
	UPROPERTY(BlueprintReadOnly, Category="BoolData|Turns/Rounds")
	int CurrentTurn = 1;

	//the amount of turns the player has left this round
	UPROPERTY(BlueprintReadOnly, Category="BoolData|Turns/Rounds")
	int TurnsThisRound = 5;

	//whether or not the player has pocketed a non cue ball and non black ball this turn
	UPROPERTY(BlueprintReadOnly, Category = "BoolData|Turns/Rounds")
	bool bHasPocketedNonFoul = false;

	//whether or not a turn is currently in progress
	UPROPERTY(BlueprintReadOnly)
	bool bTurnInProgress = false;

	//event called when the turn ends
	UFUNCTION(BlueprintImplementableEvent)
	void OnTurnEndBP();

	//event called when the round ends
	UFUNCTION(BlueprintImplementableEvent)
	void OnRoundEndBP();

	//event called when a ball is scored
	UFUNCTION(BlueprintImplementableEvent)
	void OnBallScoredBP(ABallActor* BallActor, AGoalActor* GoalActor);

	//event called when balls collide
	UFUNCTION(BlueprintImplementableEvent)
	void OnBallCollisionBP();

	//event called when a ball hits a wall
	UFUNCTION(BlueprintImplementableEvent)
	void OnWallHitBP();
};

