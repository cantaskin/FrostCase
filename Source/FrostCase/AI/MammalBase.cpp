// Fill out your copyright notice in the Description page of Project Settings.


#include "MammalBase.h"

#include "EngineUtils.h"
#include "FrostCase/GridSystem/Grid.h"
#include "FrostCase/GridSystem/GridManager.h"

// Sets default values
AMammalBase::AMammalBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMammalBase::BeginPlay()
{
	Super::BeginPlay();

	//Find GridManager
	const UWorld* World = GetWorld();
	if (World)
	{
		for (TActorIterator<AGridManager> ActorItr(World); ActorItr; ++ActorItr)
		{
			GridManager = *ActorItr;
			break;
		}
	}
	
}

// Called every frame
void AMammalBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMammalBase::Action()
{
	
}

void AMammalBase::SetGrid(AGrid* _Grid)
{
	Grid = _Grid;
}

void AMammalBase::Move()
{
	if(Grid)
	{
		
		const FVector2d &Position = Grid->GetIndex(); //Get Mammal's Grid's Index
		

		TArray<int> PossibleDirections;
		int Random = -1;
		// Add Possible Directions To Possible Direction Array and generate random index
		if((Random = GridManager->GenerateIndexFromPossibleDirections(PossibleDirections,Grid)) < 0) 
		{
			return ;
		}

		
		const int Result = PossibleDirections[Random];

		//Move To Possible Random Direction
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
 * Generic Breed Func
 */
template <typename Class, typename SubClass>
void AMammalBase::Breed(SubClass & Sub, int BreeveTurnCount) //BreveTurnCount is static
{
	//TurnCount is equalt to BreeveTurnCount
	if((TurnCount % BreeveTurnCount == 0 && Grid))
	{
		jump:
		const FVector2D &Position = Grid->GetIndex(); // Get Mammal's Grid's Index
		TArray<int> PossibleArray{};
		int Random{};

		//Generate Possible Direction Index
		if( (Random = GridManager->GenerateIndexFromPossibleDirections(PossibleArray,Grid)) < 0) 
		{
			UnableBreedCount++;
			return ;
		}
		
		FVector2D TempPos{};
		const int Result = PossibleArray[Random];

		//Choose which direction
		if(Result & North)
			TempPos = FVector2d(Position.X+1,Position.Y);
		else if(Result & East)
			TempPos = FVector2d(Position.X,Position.Y+1);
		else if(Result & South)
			TempPos = FVector2d(Position.X-1,Position.Y);
		else if(Result & West)
			TempPos = (FVector2d(Position.X,Position.Y-1));

		//Create New Mammal
		GridManager->CreateMammalGeneric<Class,SubClass>(Sub,TempPos,1);
	}
	else if(UnableBreedCount) //if we couldn't breed before 
	{
		UnableBreedCount--; 
		goto jump; // go to inside of if block
	}
}

//Looping Move Func
void AMammalBase::MoveToDest()
{
	const FVector CurrentDistance = (GetActorLocation() - (Grid->GetActorLocation() + Grid->GetGridCenterLocation()));

	if(!CurrentDistance.IsNearlyZero(1))
	{
		const FVector Direction = CurrentDistance.GetSafeNormal();
	
		FVector Location = GetActorLocation();
		Location += Direction * Speed * -1;
		SetActorLocation(Location);
	}
	else
		GetWorldTimerManager().ClearTimer(MoveTimerHandle);
}

/*
 *  Move To Dest 
 */
void AMammalBase::UpdatePositionAndMoveToDestinationPeriodically(const FVector2d& Position)
{
	Grid->SetOccupiedActor(nullptr);
	GridManager->GetEmptyGrids().Add(Grid); // Add the Mammal's ex grid to the EmptyGridArray
	
	SetGrid(GridManager->GetGrids()[Position.X][Position.Y]); // Set New Grid
	GridManager->GetEmptyGrids().Remove(Grid); //Remove the grid from EmptyGridArray
	Grid->SetOccupiedActor(this);

	//Looping Move Func
	GetWorldTimerManager().SetTimer(MoveTimerHandle,this,&AMammalBase::MoveToDest,0.05f,true); 
}

template void AMammalBase::Breed<ACat,TSubclassOf<ACat>>(TSubclassOf<ACat>& Sub,int); //linker error workaround
template void AMammalBase::Breed<AMouse,TSubclassOf<AMouse>>(TSubclassOf<AMouse>& Sub,int);