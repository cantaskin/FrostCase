// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridManager.generated.h"


class ACat;
class AMouse;
class AGrid;
class AFrostGameState;


USTRUCT()
struct FGridArray2D
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<AGrid*> GridArray;

	FGridArray2D()
	{
		
	}
	
	FGridArray2D(const TArray<AGrid*> &Array)
	{
		GridArray = Array;
	}
	
	AGrid* operator[](int Index) const
	{
		return  GridArray[Index];
	}

	void Add(AGrid *Actor)
	{
		GridArray.Add(Actor);
	}
};

UCLASS()
class FROSTCASE_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Grid Settings")
	int GridSize = 400;

	UPROPERTY(EditDefaultsOnly, Category="Grid Settings")
	int GapSize = 10;

	UPROPERTY(EditDefaultsOnly, Category="Grid Settings")
	int GridAmount = 10;


	


	UPROPERTY(EditAnywhere)
	AFrostGameState *GameState;

	UPROPERTY()
	TArray<AGrid*> EmptyGrids;
	
	UPROPERTY()
	TArray<FGridArray2D> GridArray2D; // Grid'leri ekle buraya,

	UPROPERTY(EditDefaultsOnly)
	UInstancedStaticMeshComponent* Wall;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGrid> GridClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMouse> MouseClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACat> CatClass;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void InitGrids();

	UFUNCTION(BlueprintCallable)  
	void InitWalls();

	UFUNCTION(BlueprintCallable)  
	void InitMammals();

	UFUNCTION(BlueprintCallable)  
	int GetGridAmount();
	UFUNCTION(BlueprintCallable)  
	void SetGridAmount(int Number);

	TArray<FGridArray2D> GetGrids() const;
	TArray<AGrid*> GetEmptyGrids() const;
	TSubclassOf<AMouse>& GetMouseSubClass();
	TSubclassOf<ACat> &GetCatSubClass();


	int GenerateIndexFromPossibleDirections(TArray<int>& PossibleDirections,const AGrid * Grid);
	int GenerateIndexFromPossibleDirections(TArray<int>& PossibleDirections, const FVector2d& );

	
	template <class Class, class SubClass>
	void CreateMammalGeneric(SubClass &MammalSub,int Amount, int i);

	template <class Class, class SubClass>
	void CreateMammalGeneric(SubClass& MammalSub,const FVector2d& GridIndex, int i);
};
