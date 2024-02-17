// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MammalBase.h"
#include "GameFramework/Pawn.h"
#include "Mouse.generated.h"

class AGrid;
class AGridManager;



UCLASS()
class FROSTCASE_API AMouse : public AMammalBase //public IMammal
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMouse();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	static inline int BreeveTurn = 4;// Count Turn 1 so CountTurn(1) %  BreeveTurn(4) == 0 
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Action() override;

	FTimerHandle TimerHandle_Breed;
};
