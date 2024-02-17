// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "FrostCase/FrostGameState.h"
#include "Kismet/GameplayStatics.h"

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameState = Cast<AFrostGameState>(UGameplayStatics::GetGameState(GetWorld()));

	Next->OnReleased.AddUniqueDynamic(this,&UMainWidget::Turn);
	GameState->MammalNumDelegate.AddUniqueDynamic(this,&UMainWidget::RefreshMammals); //Bind Dynamic Multicast Delegate
	
	MiceNum->SetText(FText::FromString(FString::FromInt(GameState->GetMiceNumber())));
	CatNum->SetText(FText::FromString(FString::FromInt(GameState->GetCatNumber())));
}

/*
 * 
 * 
 */
void UMainWidget::Turn()
{
	if(GameState && GameState->GetTurn() != TempTurn) // Avoid multiple trigger the NextmammalAction
	{
		TempTurn = GameState->GetTurn();
		TurnCount->SetText(FText::FromString(FString::FromInt(TempTurn + 1)));
		GameState->GetWorldTimerManager().SetTimer(GameState->TimerHandle, GameState, &AFrostGameState::NextMammalAction, 2.0f, true);
	}
}

/*
 * Refresh Mammals Text
 */
void UMainWidget::RefreshMammals()
{
	CatNum->SetText(FText::FromString(FString::FromInt(GameState->GetMammals()[0].MammalArray.Num())));
	MiceNum->SetText(FText::FromString(FString::FromInt(GameState->GetMammals()[1].MammalArray.Num())));
}
