// Fill out your copyright notice in the Description page of Project Settings.


#include "FrostPlayerController.h"

void AFrostPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
}
