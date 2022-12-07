// Fill out your copyright notice in the Description page of Project Settings.


#include "SomethingPartyGameState.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include <TileActor.h>
#include <Dice.h>
#include <SomethingParty/SomethingPartyCharacter.h>
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>



ASomethingPartyGameState::ASomethingPartyGameState()
{
	static ConstructorHelpers::FClassFinder<ADice> Dice(TEXT("/Game/TopDown/Blueprints/DiceBP"));
	if (Dice.Class != NULL)
	{
		DiceActor = Dice.Class;
	}
}

void ASomethingPartyGameState::NextTurn()
{
	CurrentTurn += 1;
	if (HasAuthority() && CurrentTurn % TurnOrder.Num() == 0) {
		GetWorld()->ServerTravel("/Game/TopDown/Maps/FloorIsLavaMap?listen");
	}
	else {
		if (HasAuthority()) {
			CurrentTurnPlayer = PlayerArray[TurnOrder[CurrentTurn % TurnOrder.Num()]];

			ASomethingPartyCharacter* Character = Cast<ASomethingPartyCharacter>(CurrentTurnPlayer->GetPawn());
			Character->SetActorLocation(Character->CurrentTile->GetActorLocation());
			//Spawn in Dice Above Player

			if (DiceActor) {
				FActorSpawnParameters spawnParams;
				spawnParams.Owner = CurrentTurnPlayer->GetPawn();
				GetWorld()->SpawnActor<ADice>(DiceActor, Character->GetActorLocation() + FVector(0, 0, 150), Character->GetActorRotation(), spawnParams);
			}

			for (APlayerState* State : PlayerArray) {
				State->GetPlayerController()->SetViewTarget(Character);
			}
		}
	}
}

void ASomethingPartyGameState::SetTurnOrder(TArray<FUniqueNetIdRepl> IDOrder)
{
	TArray<int> NewTurnOrder;
	const TArray<APlayerState*>& Players = PlayerArray;
	for (int32 Index = 0; Index != Players.Num(); ++Index)
	{
		for (int32 IDIndex = 0; IDIndex != Players.Num(); ++IDIndex) {
			if (Players[Index] && Players[Index]->GetUniqueId() == IDOrder[IDIndex])
			{
				NewTurnOrder.Insert(Index, IDIndex);
			}
		}
	}
	TurnOrder = NewTurnOrder;
}

void ASomethingPartyGameState::HandleBeginPlay()
{

	
	Super::HandleBeginPlay();
	
	CurrentTurnPlayer = PlayerArray[0];
	CurrentTurnPlayer->GetPawn()->SetActorLocation(StartTile->GetActorLocation());
	if (DiceActor) {
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = CurrentTurnPlayer->GetPawn();
		GetWorld()->SpawnActor<ADice>(DiceActor, CurrentTurnPlayer->GetPawn()->GetActorLocation() + FVector(0, 0, 150), CurrentTurnPlayer->GetPawn()->GetActorRotation(), spawnParams);
	}
}

void ASomethingPartyGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	TurnOrder.Add(PlayerArray.Num() - 1);
}

void ASomethingPartyGameState::RollDice(ASomethingPartyCharacter* Character, ADice* Dice)
{
	if (Character) {
		if (!Character->isMoving()) {
			int DiceNumber = FMath::RandRange(1, 10);
			Dice->DiceNumber = DiceNumber;
			ATileActor* CurrentTile = Character->CurrentTile;
			Character->CreateMoveSpline(CurrentTile, DiceNumber);
			MulticastMove(Character, CurrentTile, DiceNumber);
		}
	}
}

void ASomethingPartyGameState::MulticastMove_Implementation(ASomethingPartyCharacter* Character, ATileActor* CurrentTile, int num)
{
	Character->CreateMoveSpline(CurrentTile, num);
	Character->Move();
}

void ASomethingPartyGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASomethingPartyGameState, StartTile);
	DOREPLIFETIME(ASomethingPartyGameState, CurrentTurnPlayer);
}


