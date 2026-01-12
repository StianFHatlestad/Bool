// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"          // Collider example
#include "Components/StaticMeshComponent.h"
#include "HexTile.generated.h"

UCLASS()
class BOOL_API AHexTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexTile();
	
	
	UPROPERTY(EditAnywhere, Category = "Hextile")
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY()
	USphereComponent* SphereCollider;
	UPROPERTY(EditAnywhere, Category = "Hextile")
	TArray<AHexTile*> neighbours;
	
	
		 
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);
};
