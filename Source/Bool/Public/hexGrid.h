// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "hexGrid.generated.h"

UCLASS()
class BOOL_API AhexGrid : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY()
	UStaticMesh* gridMesh;
	UPROPERTY(EditAnywhere)
	int width{ 5 };
	UPROPERTY(EditAnywhere)
	int height{ 5 };
public:	
	// Sets default values for this actor's properties
	AhexGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
