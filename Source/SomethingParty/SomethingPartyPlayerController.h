// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include <SomethingParty/Public/SomethingPartyGameInstance.h>
#include "SomethingPartyPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class ASomethingPartyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASomethingPartyPlayerController();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float TurnRateGamepad;

	/*UFUNCTION(Client, Reliable)
	void GetCharacterClass();

	UFUNCTION(Server, Reliable)
		void SetCharacterClass(FCharacterInfoStruct NewChosenClass);

	UFUNCTION(BlueprintCallable, Server, Reliable)
		void RequestRespawn(FTransform const& Transform, TSubclassOf<ASomethingPartyCharacter> CharacterClass);
		*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FCharacterInfoStruct ChosenClass;
protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handles jumping */
	void Jump();
	void StopJumping();

	/**
	 * Called via input to add yaw input by a given amount.
	 * @param Val	This is the amount to add to the yaw.
	 */
	void AddControllerYawInput(float Val);

	/**
	 * Called via input to add pitch input by a given amount.
	 * @param Val	This is the amount to add to the pitch.
	 */
	void AddControllerPitchInput(float Val);

	

private:
	bool bInputPressed; // Input is bring pressed
	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
};


