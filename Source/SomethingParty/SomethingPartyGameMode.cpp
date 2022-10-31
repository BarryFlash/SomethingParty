// Copyright Epic Games, Inc. All Rights Reserved.

#include "SomethingPartyGameMode.h"
#include "SomethingPartyPlayerController.h"
#include "SomethingPartyCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

ASomethingPartyGameMode::ASomethingPartyGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ASomethingPartyPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	// set default state to our Blueprinted state
	static ConstructorHelpers::FClassFinder<AGameStateBase> SomethingPartyGameState(TEXT("/Game/TopDown/Blueprints/MyGameStateBase"));
	if (SomethingPartyGameState.Class != NULL)
	{
		GameStateClass = SomethingPartyGameState.Class;
	}
}

void ASomethingPartyGameMode::StartPlay()
{
	
	
	TArray<AActor*> ActorsToFind;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATileActor::StaticClass(), FName("StartingTile"), ActorsToFind);

	if (ActorsToFind.IsEmpty()) {
		UE_LOG(LogTemp, Fatal, TEXT("Missing start tile..."));
	}
	for (AActor* TileActor : ActorsToFind) {
		ATileActor* Tile = Cast<ATileActor>(TileActor);
		if (Tile) {
			StartTile = Tile;
		}
	}

	AGameModeBase::StartPlay();

}
