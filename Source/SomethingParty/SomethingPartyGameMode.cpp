// Copyright Epic Games, Inc. All Rights Reserved.

#include "SomethingPartyGameMode.h"
#include "SomethingPartyPlayerController.h"
#include "SomethingPartyCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include <SomethingPartyGameState.h>
#include <Dice.h>
#include "GameFramework/PlayerState.h"
#include <SomethingPartyPlayerState.h>
#include <DiceNumberWidget.h>

ASomethingPartyGameMode::ASomethingPartyGameMode()
{
	bUseSeamlessTravel = true;
	// use our custom PlayerController class
	PlayerControllerClass = ASomethingPartyPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/GoblinCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = ASomethingPartyCharacter::StaticClass();
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	// set default state to our Blueprinted state
	static ConstructorHelpers::FClassFinder<AGameStateBase> SomethingPartyGameState(TEXT("/Game/TopDown/Blueprints/SomethingPartyGameStateBP"));
	if (SomethingPartyGameState.Class != NULL)
	{
		GameStateClass = SomethingPartyGameState.Class;
	}

	// set default state to our Blueprinted player state
	static ConstructorHelpers::FClassFinder<APlayerState> SomethingPartyPlayerState(TEXT("/Game/TopDown/Blueprints/SomethingPartyPlayerStateBP"));
	if (SomethingPartyPlayerState.Class != NULL)
	{
		PlayerStateClass = SomethingPartyPlayerState.Class;
	}

	static ConstructorHelpers::FClassFinder<ADice> Dice(TEXT("/Game/TopDown/Blueprints/DiceActorBP"));
	if (Dice.Class != NULL)
	{
		DiceActor = Dice.Class;
	}

	static ConstructorHelpers::FClassFinder<APawn> StartSpot(TEXT("/Game/TopDown/Blueprints/StartSpot"));
	if (StartSpot.Class != NULL)
	{
		StartSpotPawn = StartSpot.Class;
	}
}

void ASomethingPartyGameMode::NextTurn()
{
	ASomethingPartyGameState* MainGameState = Cast<ASomethingPartyGameState>(GameState);
	if (MainGameState) {
		MainGameState->NextTurn();
	}
}

void ASomethingPartyGameMode::SetTurnOrder(TArray<ASomethingPartyPlayerState*> Order)
{
	ASomethingPartyGameState* MainGameState = Cast<ASomethingPartyGameState>(GameState);
	if (MainGameState) {
		MainGameState->SetTurnOrder(Order);
	}
}

void ASomethingPartyGameMode::RollDice(ASomethingPartyCharacter* Character, ADice* Dice)
{
	if (Character) {
		if (!Character->isMoving()) {
			int DiceNumber = FMath::RandRange(1, 10);
			Dice->DiceNumber = DiceNumber;
			FTimerDelegate Delegate;
			Delegate.BindUFunction(this, "AfterRollDice", Character, DiceNumber);
			GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, Delegate, 1, false);
			
		}
	}
}

void ASomethingPartyGameMode::AfterRollDice(ASomethingPartyCharacter* Character, int DiceNumber)
{
	UDiceNumberWidget* DiceNumWidget = Cast<UDiceNumberWidget>(Character->GetDiceNumberWidget()->GetWidget());
	if (DiceNumWidget->DiceNumberText) {
		DiceNumWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (DecidingTurns) {
		StartingTurnOrder.Add(DiceNumber, Character->GetPlayerState<ASomethingPartyPlayerState>());
		if (StartingTurnOrder.Num() == GetGameState<ASomethingPartyGameState>()->PlayerArray.Num()) {
			StartingTurnOrder.KeySort([](int A, int B) {
				return A > B;
				});
			TArray<ASomethingPartyPlayerState*> NewTurnOrder;
			for (auto& Elem : StartingTurnOrder) {
				NewTurnOrder.Add(Elem.Value);
			}
			SetTurnOrder(NewTurnOrder);
			DecidingTurns = false;
			GetGameState<ASomethingPartyGameState>()->CurrentTurnPlayer->GetPlayerController()->GetCharacter()->SetActorLocation(GetGameState<ASomethingPartyGameState>()->StartTile->GetActorLocation());
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = GetGameState<ASomethingPartyGameState>()->CurrentTurnPlayer->GetPawn();
			GetWorld()->SpawnActor<ADice>(DiceActor, GetGameState<ASomethingPartyGameState>()->CurrentTurnPlayer->GetPawn()->GetActorLocation() + FVector(0, 0, 180), GetGameState<ASomethingPartyGameState>()->CurrentTurnPlayer->GetPawn()->GetActorRotation(), spawnParams);

		}
	}
	else {
		ATileActor* CurrentTile = Character->CurrentTile;
		Character->CreateMoveSpline(CurrentTile, DiceNumber);
		GetGameState<ASomethingPartyGameState>()->MulticastMove(Character, CurrentTile, DiceNumber);
	}
}


APawn* ASomethingPartyGameMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	ASomethingPartyPlayerController* Controller = Cast<ASomethingPartyPlayerController>(NewPlayer);
	Controller->GetCharacterClass();
	return GetWorld()->SpawnActor<APawn>(StartSpotPawn, StartSpot->GetActorTransform());
}

void ASomethingPartyGameMode::Respawn(ASomethingPartyPlayerController* Controller, FTransform const& SpawnTransform, TSubclassOf<ASomethingPartyCharacter> CharacterClass)
{
	if (IsValid(Controller)) {
		ASomethingPartyCharacter* Character = GetWorld()->SpawnActor<ASomethingPartyCharacter>(CharacterClass, SpawnTransform);
		Controller->Possess(Character);
	}
}


void ASomethingPartyGameMode::StartPlay()
{

	
	DecidingTurns = true;
	TArray<AActor*> ActorsToFind;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATileActor::StaticClass(), FName("StartingTile"), ActorsToFind);

	if (ActorsToFind.IsEmpty()) {
		UE_LOG(LogTemp, Fatal, TEXT("Missing start tile..."));
	}
	for (AActor* TileActor : ActorsToFind) {
		ATileActor* Tile = Cast<ATileActor>(TileActor);
		if (Tile) {
			GetGameState<ASomethingPartyGameState>()->StartTile = Tile;
		}
	}
	
	AGameModeBase::StartPlay();
}


void ASomethingPartyGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (DiceActor && NewPlayer->GetPawn()) {
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = NewPlayer->GetPawn();
		GetWorld()->SpawnActor<ADice>(DiceActor, NewPlayer->GetPawn()->GetActorLocation() + FVector(0, 0, 150), NewPlayer->GetPawn()->GetActorRotation(), spawnParams);
	}
	//RestartPlayerAtPlayerStart(NewPlayer, FindPlayerStart(NewPlayer));
}

