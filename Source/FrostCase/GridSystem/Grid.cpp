// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Plane");
	StaticMeshComponent->SetupAttachment(RootComponent);
}

void AGrid::SetCenterLocation(const FVector &Location)
{
	GridCenterLocation = Location;
}

void AGrid::SetOccupiedActor(AActor* Mammal)
{
	OccupiedActor = Mammal;
}

void AGrid::SetPosition(const FVector &Pos)
{
	Position = Pos;
}

void AGrid::SetIndex(const FVector2d &_Index)
{
	Index = _Index;
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

