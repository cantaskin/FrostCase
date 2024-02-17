// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Containers/Queue.h"
#include "FrostGameState.generated.h"




class AMammalBase;
/**
 * 
 */

USTRUCT()
struct FMammal2DArray // can nopt tarray<tarray<>> 
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AMammalBase*> MammalArray;
	
	
	AMammalBase* operator[](int Index) const
	{
		return  MammalArray[Index];
	}

	operator bool() const
	{
		return  !MammalArray.IsEmpty();
	}

	void Add(AMammalBase *Actor)
	{
		MammalArray.Add(Actor);
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMammalNumDel);

UCLASS()
class FROSTCASE_API AFrostGameState : public AGameStateBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int TurnNumber = 0;

	UPROPERTY(EditAnywhere)
	int CatNumber = 3;

	UPROPERTY(EditAnywhere)
	int MiceNumber = 50;

	UPROPERTY(VisibleAnywhere)
	TArray<FMammal2DArray> Mammals;
	
	int CurrentRow = 0;
	int CurrentColumn = 0;


public:
	
	UPROPERTY(BlueprintAssignable)
	FMammalNumDel MammalNumDelegate;
	
	FTimerHandle TimerHandle;
	
	TArray<FMammal2DArray>& GetMammals();

	UFUNCTION(BlueprintCallable)
	int GetMiceNumber() const;
	UFUNCTION(BlueprintCallable)
	int GetCatNumber() const;

	FORCEINLINE int GetTurn() const {return TurnNumber;}

	UFUNCTION(BlueprintCallable)
	void SetMiceNumber(int Number);

	UFUNCTION(BlueprintCallable)
	void SetCatNumber(int Number);
	void NextMammalAction();
	void IncrementIndex();

	virtual void BeginPlay() override;
};
