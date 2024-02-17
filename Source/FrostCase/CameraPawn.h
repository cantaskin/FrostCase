// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

UCLASS()
class FROSTCASE_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, category = "Camera settings")
	float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, category = "Camera settings")
	float MinZoom = 500.0f;

	UPROPERTY(EditAnywhere, category = "Camera settings")
	float MaxZoom = 4000.0f;
	
	UPROPERTY(EditAnywhere, category = "Camera settings")
	float ZoomSpeed = 100.0f;

private:

	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Zoom(float Value);
	void Click();
	void MoveForward(float Value);
	void MoveRight(float Value);
};
