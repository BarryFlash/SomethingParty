// Fill out your copyright notice in the Description page of Project Settings.


#include "SomethingPartyGameState.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include <TileActor.h>
#include <Dice.h>
#include <SomethingParty/SomethingPartyCharacter.h>
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <SomethingParty/SomethingPartyGameMode.h>



ASomethingPartyGameState::ASomethingPartyGameState()
{
	
}

void ASomethingPartyGameState::NextTurn()
{
	CurrentTurn += 1;
	if (HasAuthority() && CurrentTurn % TurnOrder.Num() == 0) {
		//GetWorld()->GetTimerManager().SetTimer(TransitionTimerHandle, this, &ASomethingPartyGameState::TransitionToLevel, 5, false);
	}
	//else {
		if (HasAuthority()) {
			CurrentTurnPlayer = TurnOrder[CurrentTurn % TurnOrder.Num()];

			ASomethingPartyCharacter* Character = Cast<ASomethingPartyCharacter>(CurrentTurnPlayer->GetPawn());
			Character->SetActorLocation(Character->CurrentTile->GetActorLocation());
			//Spawn in Dice Above Player
			
			if (GetDefaultGameMode<ASomethingPartyGameMode>()->DiceActor) {
				FActorSpawnParameters spawnParams;
				spawnParams.Owner = CurrentTurnPlayer->GetPawn();
				GetWorld()->SpawnActor<ADice>(GetDefaultGameMode<ASomethingPartyGameMode>()->DiceActor, Character->GetActorLocation() + FVector(0, 0, 150), Character->GetActorRotation(), spawnParams);
			}

			for (APlayerState* State : PlayerArray) {
				State->GetPlayerController()->SetViewTarget(Character);
			}
		}
	//}
}

void ASomethingPartyGameState::SetTurnOrder_Implementation(const TArray<ASomethingPartyPlayerState*>& Order)
{
	TurnOrder = Order;
	CurrentTurnPlayer = TurnOrder[0];
	CurrentTurnPlayer->WaitingToRoll = true;
}


void ASomethingPartyGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
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

void ASomethingPartyGameState::TransitionToLevel()
{
	GetWorld()->ServerTravel("/Game/TopDown/Maps/FloorIsLavaMap?listen");
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


