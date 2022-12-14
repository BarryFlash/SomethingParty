// Fill out your copyright notice in the Description page of Project Settings.


#include "SplitTileActor.h"
#include <ArrowSelectActor.h>
#include <SomethingParty/SomethingPartyCharacter.h>
#include <SomethingPartyGameState.h>
#include "Components/TextBlock.h"
#include <DiceNumberWidget.h>
#include <SomethingParty/SomethingPartyGameMode.h>
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

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



void ASplitTileActor::OnRep_CharacterOnTile()
{
	UDiceNumberWidget* DiceNumWidget = Cast<UDiceNumberWidget>(DiceNumberWidget->GetWidget());
	if (DiceNumWidget) {
		if (CharacterOnTile) {
			if (DiceNumWidget->DiceNumberText) {
				DiceNumWidget->DiceNumberText->SetText(FText::FromString(FString::FromInt(TilesRemaining)));
				DiceNumWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else {
			DiceNumWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

}


void ASplitTileActor::TriggerAction(ASomethingPartyCharacter* Character)
{
	if (HasAuthority()) {
		CharacterOnTile = Character;
		OnRep_CharacterOnTile();
	}
	for (AArrowSelectActor* Arrow : ArrowActors) {
		if (!HasAuthority()) {
			GetWorld()->GetGameState<ASomethingPartyGameState>()->UpdateSplitTileArrowVisibility(Arrow, true);
		}
	}
}

void ASplitTileActor::SelectPath(int PathIndex)
{
	nextTile = NextTiles[PathIndex];
	for (AArrowSelectActor* Arrow : ArrowActors) {
		if (!HasAuthority()) {
			GetWorld()->GetGameState<ASomethingPartyGameState>()->UpdateSplitTileArrowVisibility(Arrow, false);
		}
	}
	if (HasAuthority()) {
		CharacterOnTile->CreateMoveSpline(this, TilesRemaining);
		Cast<ASomethingPartyGameState>(GetWorld()->GetGameState())->MulticastMove(CharacterOnTile, this, TilesRemaining);
		CharacterOnTile = NULL;
		OnRep_CharacterOnTile();
	}
	//FTimerDelegate Delegate;
	//Delegate.BindUFunction(this, "StartMoveCharacter");
	//FTimerHandle DelayTimerHandle;
	//GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, Delegate, 1, false);
	
}

void ASplitTileActor::StartMoveCharacter()
{
	if (HasAuthority()) {
		CharacterOnTile->CreateMoveSpline(this, TilesRemaining);
		Cast<ASomethingPartyGameState>(GetWorld()->GetGameState())->MulticastMove(CharacterOnTile, this, TilesRemaining);
		CharacterOnTile = NULL;
		OnRep_CharacterOnTile();
	}
}

void ASplitTileActor::BeginPlay()
{
	ArrowActors.Empty();
	for (int i = 0; i < NextTiles.Num(); i++) {
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		FVector DeltaVector = NextTiles[i]->GetActorLocation() - GetActorLocation();
		AArrowSelectActor* ArrowActorInstance = GetWorld()->SpawnActor<AArrowSelectActor>(ArrowActor, GetActorLocation() + DeltaVector / 2, DeltaVector.Rotation() + FRotator3d::MakeFromEuler(FVector(0, 0, 90)), spawnParams);
		ArrowActorInstance->PathIndex = i;
		if (!HasAuthority()) {
			GetWorld()->GetGameState<ASomethingPartyGameState>()->UpdateSplitTileArrowVisibility(ArrowActorInstance, false);
		}
		ArrowActors.Add(ArrowActorInstance);
	}
}

void ASplitTileActor::SetRemainingTiles(int Remaining)
{
	this->TilesRemaining = Remaining;
}

ASomethingPartyCharacter* ASplitTileActor::GetCharacterOnTile()
{
	return CharacterOnTile;
}

void ASplitTileActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASplitTileActor, CharacterOnTile);
}

