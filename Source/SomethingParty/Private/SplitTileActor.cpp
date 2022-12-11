// Fill out your copyright notice in the Description page of Project Settings.


#include "SplitTileActor.h"
#include <ArrowSelectActor.h>
#include <SomethingParty/SomethingPartyCharacter.h>
#include <SomethingPartyGameState.h>
#include "Components/TextBlock.h"
#include <DiceNumberWidget.h>

ASplitTileActor::ASplitTileActor() {
	static ConstructorHelpers::FClassFinder<AArrowSelectActor> ArrowActorBP(TEXT("/Game/TopDown/Blueprints/ArrowSelectActorBP"));
	if (ArrowActorBP.Class != NULL)
	{
		ArrowActor = ArrowActorBP.Class;
	}
	bReplicates = true;
}



void ASplitTileActor::TriggerAction(ASomethingPartyCharacter* Character)
{
	UDiceNumberWidget* DiceNumWidget = Cast<UDiceNumberWidget>(Character->GetDiceNumberWidget()->GetWidget());
	if (DiceNumWidget->DiceNumberText) {
		DiceNumWidget->DiceNumberText->SetText(FText::FromString(FString::FromInt(TilesRemaining)));
		DiceNumWidget->SetVisibility(ESlateVisibility::Visible);
	}
	CharacterOnTile = Character;
	ArrowActors.Empty();
	for (int i = 0; i < NextTiles.Num(); i++) {
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		FVector DeltaVector = NextTiles[i]->GetActorLocation() - GetActorLocation();
		AArrowSelectActor* ArrowActorInstance = GetWorld()->SpawnActor<AArrowSelectActor>(ArrowActor, GetActorLocation() + DeltaVector / 2, DeltaVector.Rotation() + FRotator3d::MakeFromEuler(FVector(0, 0, 90)), spawnParams);
		ArrowActorInstance->PathIndex = i;
		ArrowActors.Add(ArrowActorInstance);
	}
}

void ASplitTileActor::SelectPath(int PathIndex)
{
	UDiceNumberWidget* DiceNumWidget = Cast<UDiceNumberWidget>(CharacterOnTile->GetDiceNumberWidget()->GetWidget());
	if (DiceNumWidget->DiceNumberText) {
		DiceNumWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	nextTile = NextTiles[PathIndex];
	for (AArrowSelectActor* Arrow : ArrowActors) {
		Arrow->Destroy();
	}
	CharacterOnTile->CreateMoveSpline(this, TilesRemaining);
	Cast<ASomethingPartyGameState>(GetWorld()->GetGameState())->MulticastMove(CharacterOnTile, this, TilesRemaining);
}

void ASplitTileActor::SetRemainingTiles(int Remaining)
{
	this->TilesRemaining = Remaining;
}


