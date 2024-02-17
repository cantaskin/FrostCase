// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FrostPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FROSTCASE_API AFrostPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	
};
