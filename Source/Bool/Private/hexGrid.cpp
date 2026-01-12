// Fill out your copyright notice in the Description page of Project Settings.


#include "hexGrid.h"

// Sets default values
AhexGrid::AhexGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	HSIM = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>("HSIM");
	RootComponent = HSIM;
	
	HSIM->SetMobility(EComponentMobility::Static);
}


void AhexGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	if (tileMesh)
		HSIM->SetStaticMesh(tileMesh);
	generateHexGrid();
}

void AhexGrid::generateHexGrid()
{
	HSIM->ClearInstances();
	
	if (!tileMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("[HISM] TileMesh is NULL. Assign a StaticMesh in the grid actor."));
		return;
	}
	//Generate rows and columsn of hexagons
	const float dx = HexSize * FMath::Sqrt(3.0f) ; //Horizontal step
	const float dy = HexSize* (3.f / 2.f); //Vertical step
	
	for (int32 r = 0; r < rows; r++)
	{
		const bool bOddRow = (r&1) != 0;
		const float rowXOffset = bOddRow ? dx *0.5: 0.f;
		
		for (int32 q = 0; q < columns; q++)
		{
			const float x = rowXOffset + q * dx;
			const float y = r * dy;
			
			const FVector W (x,y,GetActorLocation().Z);
			HSIM->AddInstance(FTransform(FRotator::ZeroRotator,W,FVector(1.f)));
		}
	}
}

FVector AhexGrid::AxialToWorld(int32 q, int32 r)
{
	const float s = HexSize;
	float x=0.0f,y=0.0f;
	
	x = s*(3.f/2.f) * q;
	y = s* FMath::Sqrt(3.0f) * (r+ q* 0.5f);
	
	return FVector(x,y,GetActorLocation().Z);
}
