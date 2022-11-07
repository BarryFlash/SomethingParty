// Fill out your copyright notice in the Description page of Project Settings.

#include "FirstPersonPlayerController.h"
#include "GameFramework/Character.h"
#include "MinigamePlayerCameraManager.h"

AFirstPersonPlayerController::AFirstPersonPlayerController()
{
	// set our turn rates for input
	TurnRateGamepad = 50.f;
}

void AFirstPersonPlayerController::BeginPlay()
{
	
}


void AFirstPersonPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Bind jump events
	InputComponent->BindAction("Jump", IE_Pressed, this, &AFirstPersonPlayerController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &AFirstPersonPlayerController::StopJumping);


	// Bind movement events
	InputComponent->BindAxis("Move Forward / Backward", this, &AFirstPersonPlayerController::MoveForward);
	InputComponent->BindAxis("Move Right / Left", this, &AFirstPersonPlayerController::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn Right / Left Mouse", this, &AFirstPersonPlayerController::AddControllerYawInput);
	InputComponent->BindAxis("Look Up / Down Mouse", this, &AFirstPersonPlayerController::AddControllerPitchInput);
	InputComponent->BindAxis("Turn Right / Left Gamepad", this, &AFirstPersonPlayerController::TurnAtRate);
	InputComponent->BindAxis("Look Up / Down Gamepad", this, &AFirstPersonPlayerController::LookUpAtRate);

}

void AFirstPersonPlayerController::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		APawn* const MyPawn = GetPawn();
		if (MyPawn)
		{
			// find out which way is forward
			const FRotator Rotation = GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			MyPawn->AddMovementInput(Direction, Value);
		}
	}
}

void AFirstPersonPlayerController::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		APawn* const MyPawn = GetPawn();
		if (MyPawn)
		{
			// find out which way is right
			const FRotator Rotation = GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			// add movement in that direction
			MyPawn->AddMovementInput(Direction, Value);
		}
	}
}

void AFirstPersonPlayerController::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AFirstPersonPlayerController::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AFirstPersonPlayerController::Jump()
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		Cast<ACharacter>(MyPawn)->Jump();
	}
}

void AFirstPersonPlayerController::StopJumping()
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		Cast<ACharacter>(MyPawn)->StopJumping();
	}
}

void AFirstPersonPlayerController::AddControllerYawInput(float Val)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		MyPawn->AddControllerYawInput(Val);
	}
}

void AFirstPersonPlayerController::AddControllerPitchInput(float Val)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		MyPawn->AddControllerPitchInput(Val);
	}
}


