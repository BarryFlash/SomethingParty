// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <TileActor.h>
<<<<<<< Updated upstream
=======
#include <Dice.h>
#include <SomethingPartyPlayerState.h>
#include <SomethingParty/SomethingPartyCharacter.h>
>>>>>>> Stashed changes
#include "SomethingPartyGameMode.generated.h"

UCLASS(minimalapi)
class ASomethingPartyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASomethingPartyGameMode();

	

	
	void NextTurn();
	void SetTurnOrder(TArray<FUniqueNetIdRepl> IDOrder);

	void RollDice(ASomethingPartyCharacter* Character, class ADice* Dice);



protected:
	virtual void StartPlay() override;
<<<<<<< Updated upstream
=======

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	FTimerHandle DelayTimerHandle;

	UFUNCTION()
	void AfterRollDice(ASomethingPartyCharacter* Character, int DiceNumber);
>>>>>>> Stashed changes
};



