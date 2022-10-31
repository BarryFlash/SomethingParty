// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <TileActor.h>
#include "SomethingPartyGameMode.generated.h"

UCLASS(minimalapi)
class ASomethingPartyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASomethingPartyGameMode();

	UPROPERTY()
	ATileActor* StartTile;

protected:
	virtual void StartPlay() override;
};



