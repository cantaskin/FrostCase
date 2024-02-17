// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPawn.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ACameraPawn::ACameraPawn()
{
	
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	RootComponent = SpringArmComp;
    CameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepRelativeTransform);
    SpringArmComp->TargetArmLength = 3000.f;
    SpringArmComp->SetWorldRotation(FRotator(-70.0f, 0.f, 0.f));
    SpringArmComp->bDoCollisionTest = false;
	SpringArmComp->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Zoom", this, &ACameraPawn::Zoom);
	PlayerInputComponent->BindAction("Click",IE_Pressed, this, & ACameraPawn::Click);
	PlayerInputComponent->BindAxis("MoveForward",this,&ACameraPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this,&ACameraPawn::MoveRight);

}

void ACameraPawn::Zoom(float Value)
{
	if (Value > 0 && SpringArmComp->TargetArmLength >= MinZoom)
		SpringArmComp->TargetArmLength -= ZoomSpeed;
	else if(Value < 0 && SpringArmComp->TargetArmLength <= MaxZoom)
		SpringArmComp->TargetArmLength += ZoomSpeed;
}

void ACameraPawn::Click()
{
	
}

void ACameraPawn::MoveForward(float Value)
{
	AddActorWorldOffset(FVector(Value*MoveSpeed,0,0));
}

void ACameraPawn::MoveRight(float Value)
{
	AddActorWorldOffset(FVector(0,Value*MoveSpeed,0));
}