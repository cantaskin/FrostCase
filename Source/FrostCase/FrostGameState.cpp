// Fill out your copyright notice in the Description page of Project Settings.


#include "FrostGameState.h"
#include "AI/MammalBase.h"


TArray<FMammal2DArray>& AFrostGameState::GetMammals()
{
	return  Mammals;
}

int AFrostGameState::GetMiceNumber() const
{
	return MiceNumber;
}

int AFrostGameState::GetCatNumber() const
{
	return  CatNumber;
}

void AFrostGameState::SetMiceNumber(int Number)
{
	MiceNumber = Number;
}

void AFrostGameState::SetCatNumber(int Number)
{
	CatNumber = Number;
}


void AFrostGameState::NextMammalAction()
{
	if (Mammals.IsValidIndex(CurrentRow) && Mammals[CurrentRow].MammalArray.IsValidIndex(CurrentColumn) && Mammals[CurrentRow][CurrentColumn] != nullptr)
	{
		if (Mammals[CurrentRow][CurrentColumn]->IsValidLowLevel() && !Mammals[CurrentRow][CurrentColumn]->IsPendingKill()) //Mammal IsValid 
			Mammals[CurrentRow][CurrentColumn]->Action();
	}
	
	IncrementIndex();
}

void AFrostGameState::IncrementIndex()
{
	++CurrentColumn;
	
	if (CurrentColumn >= Mammals[CurrentRow].MammalArray.Num()) //If CurrentColumn above Column size, 
	{
		CurrentColumn = 0;
		++CurrentRow;
	}
	
	if (CurrentRow >= Mammals.Num()) //If CurrentRow above Row size. 
	{
		CurrentRow = 0;
		GetWorldTimerManager().ClearTimer(TimerHandle);
		++TurnNumber;
	}
}

void AFrostGameState::BeginPlay()
{
	Super::BeginPlay();
	
}
