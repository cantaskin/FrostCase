// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MammalBase.generated.h"

class AGrid;
class AGridManager;

enum EGridDirection
{
	North =  1 << 1, //2
	East = 1 << 2, //4
	South =  1 << 3, // 8 
	West =  1 << 4 // 16
};

UCLASS()
class FROSTCASE_API AMammalBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMammalBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int TurnCount = 1;

	int UnableBreedCount = 0;

	UPROPERTY()
	AGridManager* GridManager = nullptr;

	UPROPERTY(VisibleAnywhere)
	AGrid *Grid;
	
	UPROPERTY(EditAnywhere, Category= "Move Pawn")
	float Speed = 10.0f;

	FTimerHandle MoveTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Action();
	
	void SetGrid(AGrid* _Grid);

	FORCEINLINE AGrid* GetGrid() const 
	{
		return  Grid;
	}

protected:
	virtual void Move();
	
	template <typename Class, typename SubClass>
	void Breed(SubClass& Sub,int);
	
	void MoveToDest();
	virtual void UpdatePositionAndMoveToDestinationPeriodically(const FVector2d& Position);
};
