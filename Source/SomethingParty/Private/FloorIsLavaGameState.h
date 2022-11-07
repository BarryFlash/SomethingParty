// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/GameState.h"
#include "FloorIsLavaGameState.generated.h"

/**
 * 
 */
UCLASS()
class AFloorIsLavaGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	bool MatchStarted;
protected:

	

	float GameStartTime;
};
