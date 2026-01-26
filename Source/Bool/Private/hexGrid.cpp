// Fill out your copyright notice in the Description page of Project Settings.


#include "hexGrid.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AhexGrid::AhexGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}


void AhexGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	//Empty the grid 
	everyHexTileInTheGrid.Empty();
	//clean up al the actors
	TArray<AActor*> foundTiles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHexTile::StaticClass(),foundTiles);
	for ( AActor* actor : foundTiles )
	{
		actor->Destroy();
	}
	//Make new grid
	generateHexGrid();
}

void AhexGrid::generateHexGrid()
{
	
	
	if (!hexTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("[HISM] TileMesh is NULL. Assign a StaticMesh in the grid actor."));
		return;
	}
	
	//Generate rows and columsn of hextiles
	const float dx = HexSize * FMath::Sqrt(3.0f) ; //Horizontal step
	const float dy = HexSize* (3.f / 2.f); //Vertical step
	
	for (int32 r = 0; r < rows; r++)
	{
		const bool bOddRow = (r&1) != 0;
		const float rowXOffset = bOddRow ? dx *0.5: 0.f;
		
		//Container to keep track of grid
		TArray<AHexTile*> tiles;
		for (int32 q = 0; q < columns; q++)
		{
			const float x = rowXOffset + q * dx;
			const float y = r * dy;
			
			const FVector spawnLocation (x,y,GetActorLocation().Z);
			AHexTile* spawnedTile = GetWorld()->SpawnActor<AHexTile>(hexTile, spawnLocation, FRotator::ZeroRotator);
			if (spawnedTile)
				tiles.Add(spawnedTile);
		}
		everyHexTileInTheGrid.Add(tiles);
	}
	
	// Neighbor offsets (odd-row layout)
	const int32 EvenRowOffsets[6][2] =
	{
		{ 0, -1 }, { 0, 1 },
		{ -1, -1 }, { -1, 0 },
		{ 1, -1 }, { 1, 0 }
	};

	const int32 OddRowOffsets[6][2] =
	{
		{ 0, -1 }, { 0, 1 },
		{ -1, 0 }, { -1, 1 },
		{ 1, 0 }, { 1, 1 }
	};

	// Assign neighbors
	for (int32 Row = 0; Row < everyHexTileInTheGrid.Num(); Row++)
	{
		for (int32 Col = 0; Col < everyHexTileInTheGrid[Row].Num(); Col++)
		{
			AHexTile* Tile = everyHexTileInTheGrid[Row][Col];
			if (!IsValid(Tile)) continue;

			Tile->neighbours.Empty();

			const bool bOddRow = (Row & 1) != 0;
			const int32 (*Offsets)[2] = bOddRow ? OddRowOffsets : EvenRowOffsets;

			for (int32 i = 0; i < 6; i++)
			{
				int32 NRow = Row + Offsets[i][0];
				int32 NCol = Col + Offsets[i][1];

				if (everyHexTileInTheGrid.IsValidIndex(NRow) &&
					everyHexTileInTheGrid[NRow].IsValidIndex(NCol))
				{
					Tile->neighbours.Add(everyHexTileInTheGrid[NRow][NCol]);
				}
			}
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
