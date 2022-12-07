// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include <TileActor.h>
#include <Dice.h>
#include <SomethingParty/SomethingPartyCharacter.h>
#include "SomethingPartyGameState.generated.h"

/**
 * 
 */
UCLASS()
class ASomethingPartyGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ASomethingPartyGameState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Board)
		int MaxTurns;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Board)
		int CurrentTurn;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = Board)
		TObjectPtr<APlayerState> CurrentTurnPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Board)
		TArray<int> TurnOrder;

	UPROPERTY(Replicated)
	ATileActor* StartTile;

	
	
	void NextTurn();
	void SetTurnOrder(TArray<FUniqueNetIdRepl> IDOrder);


	/** Add PlayerState to the PlayerArray */
	virtual void AddPlayerState(APlayerState* PlayerState) override;

	void RollDice(ASomethingPartyCharacter* Character, ADice* Dice);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastMove(ASomethingPartyCharacter* Character, ATileActor* CurrentTile, int num);

};
