// Copyright Epic Games, Inc. All Rights Reserved.

#include "SomethingPartyGameMode.h"
#include "SomethingPartyPlayerController.h"
#include "SomethingPartyCharacter.h"
#include "UObject/ConstructorHelpers.h"

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
}