// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MammalBase.h"
#include "GameFramework/Pawn.h"
#include "Cat.generated.h"


class AGrid;
class AGridManager;
class AFrostGameState;

UCLASS()
class FROSTCASE_API ACat : public AMammalBase
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	inline static int BreeveTurn = 8;

	int LastEatTurn = 0;

	UPROPERTY()
	AFrostGameState* GameState;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMesh;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void Action() override;

protected:
	void Move();

	void Starve();

	void UpdatePositionAndMoveToDestinationPeriodically(const FVector2d& Position) override;
};
