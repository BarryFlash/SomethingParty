// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FloorIsLavaGameMode.generated.h"

/**
 * 
 */
UCLASS(minimalapi)
class AFloorIsLavaGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AFloorIsLavaGameMode();

	virtual void StartPlay() override;

protected:
	/** Handle for efficient management of GracePeriod timer */
	FTimerHandle WaitingTimeHandle;

	UFUNCTION()
		void OnGracePeriodOver();

};
