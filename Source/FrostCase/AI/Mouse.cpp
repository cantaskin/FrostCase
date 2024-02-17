// Fill out your copyright notice in the Description page of Project Settings.


#include "Mouse.h"
#include "FrostCase/GridSystem/GridManager.h"
#include "EngineUtils.h"
#include "FrostCase/GridSystem/Grid.h"


// Sets default values
AMouse::AMouse()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mouse");
	RootComponent = StaticMesh;

}

// Called when the game starts or when spawned
void AMouse::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMouse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMouse::Action()
{
	Move();
	
	FTimerDelegate TimerCallback;
	TimerCallback.BindLambda([this]() {
		Breed<AMouse, TSubclassOf<AMouse>>(GridManager->GetMouseSubClass(),BreeveTurn); //BreeveTurn is Static variable for Lightweight Pattern
	});

	//Run Breed Func after 2 sec 
	GetWorldTimerManager().SetTimer(TimerHandle_Breed, TimerCallback, 1.95f, false);
	TurnCount++;
}



