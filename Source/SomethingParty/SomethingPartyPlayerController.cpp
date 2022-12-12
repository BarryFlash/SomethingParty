// Copyright Epic Games, Inc. All Rights Reserved.

#include "SomethingPartyPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "SomethingPartyCharacter.h"
#include "Engine/World.h"
#include <SomethingPartyGameState.h>
#include "GameFramework/PlayerState.h"
#include <SomethingPartyPlayerState.h>
#include <SomethingParty/Public/SomethingPartyGameInstance.h>
#include <SomethingParty/SomethingPartyGameMode.h>

ASomethingPartyPlayerController::ASomethingPartyPlayerController()
{
	// set our turn rates for input
	TurnRateGamepad = 45.f;
	bEnableMouseOverEvents = true;
	bShowMouseCursor = true;

}



void ASomethingPartyPlayerController::RequestRespawn_Implementation(FTransform const& Transform, TSubclassOf<ASomethingPartyCharacter> CharacterClass)
{
	ASomethingPartyGameMode* GameMode = Cast<ASomethingPartyGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode) {
		UE_LOG(LogTemp, Warning, TEXT("CHARACTER RESPAWNED: %s"), *CharacterClass->GetName());
		GameMode->Respawn(this, Transform, CharacterClass);
	}
}

void ASomethingPartyPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Bind jump events
	InputComponent->BindAction("Jump", IE_Pressed, this, &ASomethingPartyPlayerController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ASomethingPartyPlayerController::StopJumping);


	// Bind movement events
	//InputComponent->BindAxis("Move Forward / Backward", this, &ASomethingPartyPlayerController::MoveForward);
	//InputComponent->BindAxis("Move Right / Left", this, &ASomethingPartyPlayerController::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	//InputComponent->BindAxis("Turn Right / Left Mouse", this, &ASomethingPartyPlayerController::AddControllerYawInput);
	//InputComponent->BindAxis("Look Up / Down Mouse", this, &ASomethingPartyPlayerController::AddControllerPitchInput);
	//InputComponent->BindAxis("Turn Right / Left Gamepad", this, &ASomethingPartyPlayerController::TurnAtRate);
	//InputComponent->BindAxis("Look Up / Down Gamepad", this, &ASomethingPartyPlayerController::LookUpAtRate);

}

void ASomethingPartyPlayerController::MoveForward(float Value)
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

void ASomethingPartyPlayerController::MoveRight(float Value)
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

void ASomethingPartyPlayerController::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ASomethingPartyPlayerController::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ASomethingPartyPlayerController::Jump()
{
	UE_LOG(LogTemp, Warning, TEXT("CHARACTER LATER: %s"), *ChosenClass.Name);
	bShowMouseCursor = true;
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		//if (GetPlayerState<ASomethingPartyPlayerState>()->WaitingToRoll  && !Cast<ASomethingPartyCharacter>(MyPawn)->isMoving()) {
			Cast<ACharacter>(MyPawn)->Jump();
		//}
	}
}

void ASomethingPartyPlayerController::StopJumping()
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		Cast<ACharacter>(MyPawn)->StopJumping();
	}
}

void ASomethingPartyPlayerController::AddControllerYawInput(float Val)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		MyPawn->AddControllerYawInput(Val);
	}
}

void ASomethingPartyPlayerController::AddControllerPitchInput(float Val)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		MyPawn->AddControllerPitchInput(Val);
	}
}

void ASomethingPartyPlayerController::GetCharacterClass_Implementation()
{
	
	ChosenClass = GetGameInstance<USomethingPartyGameInstance>()->ChosenClass;
	//SetCharacterClass(ChosenClass);
}

void ASomethingPartyPlayerController::SetCharacterClass_Implementation(FCharacterInfoStruct NewChosenClass)
{
	ChosenClass = NewChosenClass;
}
