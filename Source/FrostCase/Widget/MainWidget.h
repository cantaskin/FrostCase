// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

class AFrostGameState;
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class FROSTCASE_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* MiceNum;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* CatNum;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* TurnCount;

	UPROPERTY(meta=(BindWidget))
	UButton* Next;

	UPROPERTY()
	AFrostGameState* GameState;

	int TempTurn = -1;

public:

	virtual void NativeConstruct() override;

	UFUNCTION()
	void Turn();

	UFUNCTION()
	void RefreshMammals();
};
