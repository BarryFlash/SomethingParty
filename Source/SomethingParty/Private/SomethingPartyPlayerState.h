// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include <Dice.h>
#include "SomethingPartyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ASomethingPartyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ADice* Dice;
	bool WaitingToRoll;


protected:

	virtual void BeginPlay() override;
};
