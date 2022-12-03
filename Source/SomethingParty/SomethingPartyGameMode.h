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

	

	
	void NextTurn();
	void SetTurnOrder(TArray<FUniqueNetIdRepl> IDOrder);

	void RollDice(class ASomethingPartyCharacter* Character, class ADice* Dice);



protected:
	virtual void StartPlay() override;
};



