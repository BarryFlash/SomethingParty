// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerState.h"
#include <SomethingPartyPlayerState.h>
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

void ALobbyPlayerState::CopyProperties(APlayerState* PlayerState)
{
	APlayerState::CopyProperties(PlayerState);
	UE_LOG(LogTemp, Warning, TEXT("COPYING PROPERTIES!"));
	if (IsValid(PlayerState)) {
		ASomethingPartyPlayerState* GamePlayerState = Cast<ASomethingPartyPlayerState>(PlayerState);
		
		if (GamePlayerState) {
			UE_LOG(LogTemp, Warning, TEXT("COPYING PROPERTIES! OLD: %s"), *CharacterInfo.Name);
			GamePlayerState->CharacterInfo = CharacterInfo;
		}
	}
}

void ALobbyPlayerState::SetCharacterInfo(FCharacterInfoStruct NewCharacterInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("SETTING CHAR INFO: %s"), *NewCharacterInfo.Name);
	CharacterInfo = NewCharacterInfo;
}

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ALobbyPlayerState, CharacterInfo);
}
