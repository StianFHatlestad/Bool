// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexTile.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "hexGrid.generated.h"

UCLASS()
class BOOL_API AhexGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AhexGrid();
	
private:
	void generateHexGrid();
	FVector AxialToWorld(int32 q, int32 r);
public:	
	UPROPERTY(EditAnywhere, Category="Hex", meta=(ClampMin="1"))
	int32 rows{ 5 };
	UPROPERTY(EditAnywhere, Category = "Hex", meta=(ClampMin="1"))
	int32 columns{ 5 };
	UPROPERTY(EditAnywhere, Category="Hex", meta=(ClampMin="1.0"))
	float HexSize = 100.f;
	
	//Array of all the tiles
	TArray<TArray<AHexTile*>> everyHexTileInTheGrid;
	
	UPROPERTY(EditAnywhere, Category = "Hex")
	TSubclassOf<AHexTile> hexTile;
	virtual void OnConstruction(const FTransform& Transform) override;
};
