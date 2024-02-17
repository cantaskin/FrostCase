// Fill out your copyright notice in the Description page of Project Settings.


#include "Cat.h"
#include "Mouse.h"
#include "EngineUtils.h"
#include "FrostCase/FrostGameState.h"
#include "FrostCase/GridSystem/Grid.h"
#include "FrostCase/GridSystem/GridManager.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ACat::ACat()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Cat");
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void ACat::BeginPlay()
{
	Super::BeginPlay();
	GameState = Cast<AFrostGameState>(UGameplayStatics::GetGameState(GetWorld()));	
}

// Called every frame
void ACat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACat::Move()
{
	if(Grid)
	{
		const FVector2d &Position = Grid->GetIndex();
		

		TArray<int> PossibleDirections;
		
		int Random = -1;

		//Add to PossibleDirectionArray where the mice is 
		if((Random = GridManager->GenerateIndexFromPossibleDirections(PossibleDirections,GetGrid()->GetIndex())) < 0) 
		{
			LastEatTurn++;
			if((Random = GridManager->GenerateIndexFromPossibleDirections(PossibleDirections,Grid)) < 0)
				return ;
		}
		else
			LastEatTurn = 0;

		int Result = PossibleDirections[Random];

		//Find Possible Direction and move
		if(Result & North)
			UpdatePositionAndMoveToDestinationPeriodically(FVector2d(Position.X+1,Position.Y));
		else if(Result & East)
			UpdatePositionAndMoveToDestinationPeriodically(FVector2d(Position.X,Position.Y+1));
		else if(Result & South)
			UpdatePositionAndMoveToDestinationPeriodically(FVector2d(Position.X-1,Position.Y));
		else if(Result & West)
			UpdatePositionAndMoveToDestinationPeriodically(FVector2d(Position.X,Position.Y-1));
	}
}
/*
 * Cat Dies
 */ 
void ACat::Starve()
{
	UE_LOG(LogTemp,Warning,TEXT("Cat Dead"));
	if(LastEatTurn == 3 && GameState)
	{
			for(auto &MammalArray : GameState->GetMammals())
			{
				if(MammalArray.MammalArray.Find(this) >= 0)
				{
					//Cat remove from mammal array and trigger text refresh event
					MammalArray.MammalArray.Remove(this);
					GameState->MammalNumDelegate.Broadcast();
				}
			}
			Grid->SetOccupiedActor(nullptr);
			Destroy();
	}
}
/*
 * Eat Mouse
 */
void ACat::UpdatePositionAndMoveToDestinationPeriodically(const FVector2d& Position)
{
	Grid->SetOccupiedActor(nullptr);
	GridManager->GetEmptyGrids().Add(Grid);
	SetGrid(GridManager->GetGrids()[Position.X][Position.Y]);

	if(Grid->GetOccupiedActor() && GameState)
	{
			for(auto &MammalArray : GameState->GetMammals())
			{
				if(const int i = MammalArray.MammalArray.Find(Cast<AMouse>(Grid->GetOccupiedActor())) >= 0)
				{
					//Eat Mouse and trigger text event
					MammalArray.MammalArray.Remove(Cast<AMouse>(Grid->GetOccupiedActor()));
					GameState->MammalNumDelegate.Broadcast();
				}
			}
			Grid->GetOccupiedActor()->Destroy();
	}
	else
		GridManager->GetEmptyGrids().Remove(Grid);

	Grid->SetOccupiedActor(this); 
	GetWorldTimerManager().SetTimer(MoveTimerHandle,this,&ACat::MoveToDest,0.05f,true);
}

/*void ACat::Action()
{
	UE_LOG(LogTemp,Warning,TEXT("Cat Action"));	
	Move();
	Breed<ACat,TSubclassOf<ACat>>(GridManager->GetCatSubClass());
	Starve();
}*/

void ACat::Action()
{
	Move();
	
	FTimerDelegate TimerCallback;
	TimerCallback.BindLambda([this]() {
		Breed<ACat, TSubclassOf<ACat>>(GridManager->GetCatSubClass(),BreeveTurn); //BreeveTurn is Static variable for Lightweight Pattern
		Starve();
	});

	//Run Breed Func after 2 sec
	FTimerHandle TimerHandle_Breed;
	GetWorldTimerManager().SetTimer(TimerHandle_Breed, TimerCallback, 1.95f, false);
	TurnCount++;
}


