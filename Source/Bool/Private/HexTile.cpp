// Fill out your copyright notice in the Description page of Project Settings.


#include "HexTile.h"

#include "Balls/BallActor.h"

// Sets default values
AHexTile::AHexTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = sceneComponent;
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->InitSphereRadius(100.0f);
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollider->SetGenerateOverlapEvents(true);
	SphereCollider->SetupAttachment(RootComponent);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AHexTile::OnOverlapBegin);
}

void AHexTile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		AActor* actor = static_cast<AActor*>(OtherActor);
		if (actor != nullptr && ballOnTile == nullptr )
		{
			ballOnTile = actor;
		}
	}
}

