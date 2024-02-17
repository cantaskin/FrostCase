// Fill out your copyright notice in the Description page of Project Settings.


#include "GridManager.h"
#include "Grid.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "FrostCase/FrostGameState.h"
#include "FrostCase/AI/Cat.h"
#include "..\AI\Mouse.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGridManager::AGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Wall = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Wall");
	Wall->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();
	GameState = Cast<AFrostGameState>(UGameplayStatics::GetGameState(GetWorld()));
	
	
}

// Called every frame
void AGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
/*
 * Create Grids
 */ 
void AGridManager::InitGrids()
{
	AGrid *LocalGrid{};
	for(int x = 0; x < GridAmount; x++)
	{
		FGridArray2D LocalGridArray;
		for(int y = 0; y < GridAmount; y++)
		{
			FVector Grid;
			Grid.X = x * (GridSize + GapSize); 
			Grid.Y = y * (GridSize + GapSize);
			
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			LocalGrid = GetWorld()->SpawnActor<AGrid>(GridClass, Grid, FRotator(0, 0, 0), SpawnInfo);
			LocalGrid->SetPosition(FVector(Grid.X,Grid.Y,0));
			LocalGrid->SetIndex(FVector2d(x,y));
			LocalGridArray.Add(LocalGrid);
			EmptyGrids.Add(LocalGrid);
		}
		GridArray2D.Add(LocalGridArray);
	}
	if(LocalGrid)
		LocalGrid->SetCenterLocation(FVector(GridSize/2,GridSize/2,0)); 
}

void AGridManager::InitWalls()
{
	for(int x = 0; x < GridAmount; x++)
	{
		if(GridArray2D.IsValidIndex(x))
		{
			//West Walls
			if(GridArray2D[x].GridArray.IsValidIndex(0))
			{
				FVector WallWestLocation = GridArray2D[x][0]->GetActorLocation() + FVector(0.0f,-GapSize,0.0f) ;
				FTransform WallWestTransform(FRotator(0.0f,0.0f,0.0f),WallWestLocation);
				Wall->AddInstance(WallWestTransform,true);
			}
			//East Walls
			if(GridArray2D[x].GridArray.IsValidIndex(GridAmount -1))
			{
				FVector WallEastLocation = GridArray2D[x][GridAmount - 1]->GetActorLocation() + FVector(0.0f,GridSize + GapSize,0.0f);
				FTransform WallEastTransform(FRotator(0.0f,0.0f,0.0f),WallEastLocation);
				Wall->AddInstance(WallEastTransform,true);
			}
		}
		//South Walls
		if(GridArray2D.IsValidIndex(0) &&  GridArray2D[0].GridArray.IsValidIndex(x))
		{
			FVector WallSouthLocation = GridArray2D[0][x]->GetActorLocation() + FVector(-GapSize,0.0f,0.0f) ;
			FTransform WallSouthTransform(FRotator(0.0f,90.0f,0.0f),WallSouthLocation);
			Wall->AddInstance(WallSouthTransform,true);
		}

		//North Walls
		if(GridArray2D.IsValidIndex(GridAmount - 1) &&  GridArray2D[GridAmount -1].GridArray.IsValidIndex(x))
		{
			FVector WallNorthLocation = GridArray2D[GridAmount - 1][x]->GetActorLocation() +  FVector(GridSize + GapSize,0.0f,0.0f) ;
			UE_LOG(LogTemp,Warning,TEXT("%f, %f, %f"),WallNorthLocation.X,WallNorthLocation.Y,WallNorthLocation.Z);
			FTransform WallNorthTransform(FRotator(0.0f,90.0f,0.0f),WallNorthLocation);
			Wall->AddInstance(WallNorthTransform,true);
		}
	}
}

void AGridManager::InitMammals()
{
	if(GameState) //GameState Check and then Create Cats
	{
		GameState->GetMammals().SetNum(2); // Reserve 2 row . First Row for Cats , Second Row for Mice.
		CreateMammalGeneric<ACat,TSubclassOf<ACat>>(CatClass,GameState->GetCatNumber(),0);
		CreateMammalGeneric<AMouse,TSubclassOf<AMouse>>(MouseClass,GameState->GetMiceNumber(),1);
	}
	
	
}

int AGridManager::GetGridAmount()
{
	return  GridAmount;
}

void AGridManager::SetGridAmount(int Number)
{
	GridAmount = Number;
}

/*
 *	Generate Index with Occupied Check
 */

TArray<FGridArray2D> AGridManager::GetGrids() const
{
	return GridArray2D;
}

TArray<AGrid*> AGridManager::GetEmptyGrids() const
{
	return EmptyGrids;
}

TSubclassOf<ACat>& AGridManager::GetCatSubClass()
{
	return CatClass;
}

TSubclassOf<AMouse>& AGridManager::GetMouseSubClass()
{
	return MouseClass;
}

/*
 * Generate Index From PossibleDirections for Move
 */
int AGridManager::GenerateIndexFromPossibleDirections(TArray<int>& PossibleDirections,const AGrid * Grid)
{
	const FVector2d &Position = Grid->GetIndex();
	if(GridArray2D.IsValidIndex(Position.X + 1) && GridArray2D[Position.X+1].GridArray.IsValidIndex(Position.Y) && !GridArray2D[Position.X+1][Position.Y]->GetOccupiedActor())
		PossibleDirections.Add(North);
	if(GridArray2D.IsValidIndex(Position.X) && GridArray2D[Position.X].GridArray.IsValidIndex(Position.Y+1) && !GridArray2D[Position.X][Position.Y+1]->GetOccupiedActor())
		PossibleDirections.Add(East);
	if(GridArray2D.IsValidIndex(Position.X - 1) &&  GridArray2D[Position.X -1].GridArray.IsValidIndex(Position.Y)  && !GridArray2D[Position.X-1][Position.Y]->GetOccupiedActor())
		PossibleDirections.Add(South);
	if(GridArray2D.IsValidIndex(Position.X) && GridArray2D[Position.X].GridArray.IsValidIndex(Position.Y-1) && !GridArray2D[Position.X][Position.Y-1]->GetOccupiedActor())
		PossibleDirections.Add(West);

	if(PossibleDirections.Num() <= 0)
		return -1;
		
	return FMath::RandRange(0,PossibleDirections.Num() -1);
}

/*
 * Generate Index From Possible Direction For Eat Mice
 */
int AGridManager::GenerateIndexFromPossibleDirections(TArray<int>& PossibleDirections, const FVector2d& Position)
{
	{
		if(GridArray2D.IsValidIndex(Position.X + 1) && GridArray2D[Position.X+1].GridArray.IsValidIndex(Position.Y) && Cast<AMouse>(GridArray2D[Position.X+1][Position.Y]->GetOccupiedActor()))
			PossibleDirections.Add(North);
		if(GridArray2D.IsValidIndex(Position.X) && GridArray2D[Position.X].GridArray.IsValidIndex(Position.Y+1) && Cast<AMouse>(GridArray2D[Position.X][Position.Y+1]->GetOccupiedActor()))
			PossibleDirections.Add(East);
		if(GridArray2D.IsValidIndex(Position.X - 1) &&  GridArray2D[Position.X -1].GridArray.IsValidIndex(Position.Y) && Cast<AMouse>(GridArray2D[Position.X-1][Position.Y]->GetOccupiedActor()))
			PossibleDirections.Add(South);
		if(GridArray2D.IsValidIndex(Position.X) && GridArray2D[Position.X].GridArray.IsValidIndex(Position.Y-1) && Cast<AMouse>(GridArray2D[Position.X][Position.Y-1]->GetOccupiedActor()))
			PossibleDirections.Add(West);
    
		if(PossibleDirections.Num() <= 0)
			return -1;
    		
		return FMath::RandRange(0,PossibleDirections.Num() -1);
	}
}


/*
 * Generic Func for Spawn Mammals random empty locations
 */

template <class Class, class SubClass>
void AGridManager::CreateMammalGeneric(SubClass& MammalSub, int Amount, int i)
{
	for(int index = 0; index < Amount; index++)
	{
		const int EmptyGridRandomIndex = FMath::RandRange(0,EmptyGrids.Num()-1); //Generate Random Index for EmptyGridArray
		const AGrid *Grid = EmptyGrids[EmptyGridRandomIndex]; 
		const FVector2d GridIndex = Grid->GetIndex();

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		Class *Mammal = GetWorld()->SpawnActor<Class>(MammalSub, GridArray2D[GridIndex.X][GridIndex.Y]->GetGridCenterLocation() + GridArray2D[GridIndex.X][GridIndex.Y]->GetActorLocation(), FRotator(0, 0, 0), SpawnInfo);
		GridArray2D[GridIndex.X][GridIndex.Y]->SetOccupiedActor(Mammal); 
		Mammal->SetGrid(GridArray2D[GridIndex.X][GridIndex.Y]);
		EmptyGrids.Remove(Mammal->GetGrid());
		GameState->GetMammals()[i].MammalArray.Add(Mammal); // Add Mammal to Mammals Array. First Row for Cats, Second Row for Mice 
	}
}

/*
 * Generic Func for Spawn Mammal with Location
 */

template <class Class, class SubClass>
void AGridManager::CreateMammalGeneric(SubClass& MammalSub,const FVector2d& GridIndex, int i)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	Class *Mammal = GetWorld()->SpawnActor<Class>(MammalSub, GridArray2D[GridIndex.X][GridIndex.Y]->GetGridCenterLocation() + GridArray2D[GridIndex.X][GridIndex.Y]->GetActorLocation(), FRotator(0, 0, 0), SpawnInfo);
	GridArray2D[GridIndex.X][GridIndex.Y]->SetOccupiedActor(Mammal); 
	Mammal->SetGrid(GridArray2D[GridIndex.X][GridIndex.Y]);
	EmptyGrids.Remove(Mammal->GetGrid());
	GameState->GetMammals()[i].MammalArray.Add(Mammal);
	GameState->MammalNumDelegate.Broadcast();
}


template void AGridManager::CreateMammalGeneric<AMouse,TSubclassOf<AMouse>>(TSubclassOf<AMouse>& MammalSub,const FVector2d& GridIndex, int i); //linker error workaround
template void AGridManager::CreateMammalGeneric<ACat,TSubclassOf<ACat>>(TSubclassOf<ACat>& MammalSub,const FVector2d& GridIndex, int i);
template void AGridManager::CreateMammalGeneric<AMouse,TSubclassOf<AMouse>>(TSubclassOf<AMouse>& MammalSub, int Amount, int i);
template void AGridManager::CreateMammalGeneric<ACat,TSubclassOf<ACat>>(TSubclassOf<ACat>& MammalSub, int Amount, int i);

