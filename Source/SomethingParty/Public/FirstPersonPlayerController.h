// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FirstPersonPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AFirstPersonPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AFirstPersonPlayerController();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float TurnRateGamepad;


protected:

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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	bool bInputPressed; // Input is bring pressed
	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
};
