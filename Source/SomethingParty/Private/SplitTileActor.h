// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileActor.h"
#include "TriggerableTileInterface.h"
#include "ArrowSelectActor.h"
#include <SomethingParty/SomethingPartyCharacter.h>
#include "SplitTileActor.generated.h"

/**
 * 
 */
UCLASS()
class ASplitTileActor : public ATileActor, public ITriggerableTileInterface
{
	GENERATED_BODY()
	

public:
	ASplitTileActor();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Options")
	TArray<ATileActor*> NextTiles;

	UPROPERTY()
	TArray<AArrowSelectActor*> ArrowActors;

	UPROPERTY()
	TSubclassOf<AArrowSelectActor> ArrowActor;

	int TilesRemaining;

	UPROPERTY()
	ASomethingPartyCharacter* CharacterOnTile;

	UPROPERTY(VisibleAnywhere, Category = "Dice")
		class UWidgetComponent* DiceNumberWidget;

public:
	virtual void TriggerAction(ASomethingPartyCharacter* Character) override;
	void SelectPath(int PathIndex);
	void SetRemainingTiles(int Remaining);
	ASomethingPartyCharacter* GetCharacterOnTile();
};
