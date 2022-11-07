// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorIsLavaGameMode.h"
#include "FloorIsLavaGameState.h"
#include "FirstPersonPlayerController.h"
#include "SomethingParty/SomethingPartyCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

AFloorIsLavaGameMode::AFloorIsLavaGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AFirstPersonPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/MinigameCharacterBP"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/FirstPersonPlayerControllerBP"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	// set default state to our Blueprinted state
	static ConstructorHelpers::FClassFinder<AGameStateBase> SomethingPartyGameState(TEXT("/Game/TopDown/Blueprints/FloorIsLavaGameStateBP"));
	if (SomethingPartyGameState.Class != NULL)
	{
		GameStateClass = SomethingPartyGameState.Class;
	}

}

void AFloorIsLavaGameMode::StartPlay()
{
	AGameModeBase::StartPlay();
	GetWorldTimerManager().SetTimer(WaitingTimeHandle, this, &AFloorIsLavaGameMode::OnGracePeriodOver, 20, false);
}

void AFloorIsLavaGameMode::OnGracePeriodOver()
{
	Cast<AFloorIsLavaGameState>(GameState)->MatchStarted = true;
	GetWorldTimerManager().ClearTimer(WaitingTimeHandle);
}

