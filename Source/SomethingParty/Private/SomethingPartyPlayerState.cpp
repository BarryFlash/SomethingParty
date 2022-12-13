// Fill out your copyright notice in the Description page of Project Settings.


#include "SomethingPartyPlayerState.h"
#include <LobbyPlayerState.h>

void ASomethingPartyPlayerState::BeginPlay()
{
	Super::BeginPlay();
	WaitingToRoll = true;
}


