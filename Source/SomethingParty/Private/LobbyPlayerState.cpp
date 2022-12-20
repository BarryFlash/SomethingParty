// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerState.h"
#include <SomethingPartyPlayerState.h>
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

void ALobbyPlayerState::CopyProperties(APlayerState* PlayerState)
{
	APlayerState::CopyProperties(PlayerState);
	if (IsValid(PlayerState)) {
		ASomethingPartyPlayerState* GamePlayerState = Cast<ASomethingPartyPlayerState>(PlayerState);
		if (GamePlayerState) {
			GamePlayerState->CharacterInfo = CharacterInfo;
		}
	}
}

void ALobbyPlayerState::SetCharacterInfo(FCharacterInfoStruct NewCharacterInfo)
{
	CharacterInfo = NewCharacterInfo;
}

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ALobbyPlayerState, CharacterInfo);
}
