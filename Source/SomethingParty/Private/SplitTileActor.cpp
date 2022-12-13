// Fill out your copyright notice in the Description page of Project Settings.


#include "SplitTileActor.h"
#include <ArrowSelectActor.h>
#include <SomethingParty/SomethingPartyCharacter.h>
#include <SomethingPartyGameState.h>
#include "Components/TextBlock.h"
#include <DiceNumberWidget.h>
#include <SomethingParty/SomethingPartyGameMode.h>

ASplitTileActor::ASplitTileActor() {
	static ConstructorHelpers::FClassFinder<AArrowSelectActor> ArrowActorBP(TEXT("/Game/TopDown/Blueprints/ArrowSelectActorBP"));
	if (ArrowActorBP.Class != NULL)
	{
		ArrowActor = ArrowActorBP.Class;
	}
	bReplicates = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	DiceNumberWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DiceNumber"));
	DiceNumberWidget->SetupAttachment(RootComponent);
	DiceNumberWidget->SetWidgetSpace(EWidgetSpace::Screen);
	DiceNumberWidget->SetDrawSize(FVector2D(100, 150));
	static ConstructorHelpers::FClassFinder<UUserWidget> DiceResultUI(TEXT("/Game/TopDown/Blueprints/DiceResultUI"));
	if (DiceResultUI.Class != NULL)
	{
		DiceNumberWidget->SetWidgetClass(DiceResultUI.Class);
	}
}



void ASplitTileActor::TriggerAction(ASomethingPartyCharacter* Character)
{
	if (HasAuthority()) {
		ASomethingPartyGameMode* GameMode = Cast<ASomethingPartyGameMode>(GetWorld()->GetAuthGameMode());
		GameMode->UpdateDiceNumberWidget(DiceNumberWidget, TilesRemaining, true);
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
	if (HasAuthority()) {
		ASomethingPartyGameMode* GameMode = Cast<ASomethingPartyGameMode>(GetWorld()->GetAuthGameMode());
		GameMode->UpdateDiceNumberWidget(DiceNumberWidget, NULL, false);
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

ASomethingPartyCharacter* ASplitTileActor::GetCharacterOnTile()
{
	return CharacterOnTile;
}


