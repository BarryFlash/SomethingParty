// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include <SomethingParty/Public/SomethingPartyGameInstance.h>
#include "LobbyPlayerState.generated.h"


/**
 * 
 */
UCLASS()
class ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	virtual void CopyProperties(APlayerState* PlayerState) override;

public:
	UPROPERTY(Replicated)
	FCharacterInfoStruct CharacterInfo;

	UFUNCTION(BlueprintCallable)
		void SetCharacterInfo(FCharacterInfoStruct NewCharacterInfo);
};
