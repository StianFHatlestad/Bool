// Fill out your copyright notice in the Description page of Project Settings.


#include "Balls/CueBall.h"

ACueBall::ACueBall()
{
	////enable ticking
	//PrimaryActorTick.bCanEverTick = true;
}

void ACueBall::BeginPlay()
{
	//call the parent implementation
	Super::BeginPlay();

	//set the start position
	StartPosition = GetActorLocation();
}

