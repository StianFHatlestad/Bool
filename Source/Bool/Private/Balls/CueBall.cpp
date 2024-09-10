// Fill out your copyright notice in the Description page of Project Settings.


#include "Balls/CueBall.h"

#include "Components/SphereComponent.h"

ACueBall::ACueBall()
{
	//enable ticking
	PrimaryActorTick.bCanEverTick = true;

	//create the component(s)
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Root"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	//set the sphere component to be the root component
	RootComponent = SphereComponent;

	//set parameters for the sphere component
	SphereComponent->InitSphereRadius(50.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	SphereComponent->SetSimulatePhysics(true);
	SphereComponent->SetLinearDamping(0.1f);
	SphereComponent->SetAngularDamping(0.1f);
	SphereComponent->SetEnableGravity(true);
	SphereComponent->SetConstraintMode(EDOFMode::Type::XYPlane);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	//bind the OnHit event
	SphereComponent->OnComponentHit.AddDynamic(this, &ACueBall::OnSphereHit);
}

void ACueBall::OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

}

