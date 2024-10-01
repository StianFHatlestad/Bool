// Fill out your copyright notice in the Description page of Project Settings.


#include "Balls/BallActor.h"

#include "Balls/BallUpgradeDataAsset.h"
#include "Balls/CueBall.h"
#include "Bool/GoalActor.h"
#include "Components/SphereComponent.h"


ABallActor::ABallActor()
{
	////enable ticking
	//PrimaryActorTick.bCanEverTick = true;

	//create the component(s)
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	//set the sphere component to be the root component
	RootComponent = SphereComponent;

	//setup attachment(s)
	MeshComponent->SetupAttachment(RootComponent);

	////set parameters for the sphere component
	//SphereComponent->InitSphereRadius(50.0f);
	//SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	//SphereComponent->SetSimulatePhysics(true);
	//SphereComponent->SetLinearDamping(0.1f);
	//SphereComponent->SetAngularDamping(0.1f);
	//SphereComponent->SetEnableGravity(true);
	////SphereComponent->SetConstraintMode(EDOFMode::Type::XYPlane);
	//SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//SphereComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	//bind the OnHit event
	SphereComponent->OnComponentHit.AddDynamic(this, &ABallActor::OnSphereHit);
}

void ABallActor::BeginPlay()
{
	//call the parent implementation
	Super::BeginPlay();

	//set the start position
	StartPosition = GetActorLocation();
}

void ABallActor::OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//check if the other actor is a ball actor
	if (OtherActor->IsA(StaticClass()))
	{
		//check if the other actor is a cue ball
		if (OtherActor->IsA(ACueBall::StaticClass()))
		{
			//get the cue ball
			ACueBall* CueBall = Cast<ACueBall>(OtherActor);

			//loop through the ball upgrade data assets
			for (UBallUpgradeDataAsset* BallUpgradeDataAsset : BallUpgradeDataAssets)
			{
				//call the OnCueBallHit function
				BallUpgradeDataAsset->OnCueBallHit(this, HitComponent, CueBall, OtherComp, NormalImpulse, Hit);
			}
		}
		else
		{
			//get the other ball actor
			ABallActor* OtherBallActor = Cast<ABallActor>(OtherActor);

			//loop through the ball upgrade data assets
			for (UBallUpgradeDataAsset* BallUpgradeDataAsset : BallUpgradeDataAssets)
			{
				//call the OnNormalBallHit function
				BallUpgradeDataAsset->OnNormalBallHit(this, HitComponent, OtherBallActor, OtherComp, NormalImpulse, Hit);
			}
		}
	}
	else
	{
		//loop through the ball upgrade data assets
		for (UBallUpgradeDataAsset* BallUpgradeDataAsset : BallUpgradeDataAssets)
		{
			//call the OnWallHit function
			BallUpgradeDataAsset->OnWallHit(this, HitComponent, Hit);
		}
	}
}

