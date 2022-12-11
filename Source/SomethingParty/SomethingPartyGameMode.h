// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <TileActor.h>
#include <Dice.h>
#include <SomethingPartyPlayerState.h>
#include <SomethingParty/SomethingPartyCharacter.h>
#include "SomethingPartyGameMode.generated.h"

UCLASS(minimalapi)
class ASomethingPartyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASomethingPartyGameMode();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dice")
	TSubclassOf<ADice> DiceActor;
	
	void NextTurn();
	void SetTurnOrder(TArray<ASomethingPartyPlayerState*> Order);

	void RollDice(ASomethingPartyCharacter* Character, class ADice* Dice);

	bool DecidingTurns;

	TMultiMap<int, ASomethingPartyPlayerState*> StartingTurnOrder;

protected:
	virtual void StartPlay() override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	FTimerHandle DelayTimerHandle;

	UFUNCTION()
	void AfterRollDice(ASomethingPartyCharacter* Character, int DiceNumber);
};



