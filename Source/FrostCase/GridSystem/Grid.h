// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Vector2D.h"
#include "Grid.generated.h"


UCLASS()
class FROSTCASE_API AGrid : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComponent;
	
	inline static FVector GridCenterLocation; //Flyweight pattern

	UPROPERTY(VisibleAnywhere)
	FVector Position;

	UPROPERTY(VisibleAnywhere)
	FVector2D Index;
	
	UPROPERTY(VisibleAnywhere)
	AActor* OccupiedActor;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AGrid();
	// Called every frame
	void SetCenterLocation(const FVector &Location);
	void SetOccupiedActor(AActor* Mammal);
	void SetPosition(const FVector &pos);
	void SetIndex(const FVector2d &_Index);
	virtual void Tick(float DeltaTime) override;

	//Getters
	FORCEINLINE FVector GetGridCenterLocation() const{return GridCenterLocation;}
	FORCEINLINE AActor* GetOccupiedActor() const{return OccupiedActor;}
	FORCEINLINE FVector GetPosition() const{return  Position;}
	FORCEINLINE FVector2D GetIndex () const {return Index;}
};
