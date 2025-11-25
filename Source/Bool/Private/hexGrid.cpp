// Fill out your copyright notice in the Description page of Project Settings.


#include "hexGrid.h"

// Sets default values
AhexGrid::AhexGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AhexGrid::BeginPlay()
{
	Super::BeginPlay();

	//Create grid
	for (int i = 0; i < width; i++)
	{	
		for (int j = 0; j < height; j++)
		{
			//spawn meshes 
		}
	}
}

// Called every frame
void AhexGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

