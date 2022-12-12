// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <Runtime/Engine/Classes/Engine/UserDefinedStruct.h>
#include <SomethingParty/SomethingPartyCharacter.h>
#include "Engine/DataTable.h"
#include "SomethingPartyGameInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FCharacterInfoStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASomethingPartyCharacter> Character;
};

UCLASS()
class SOMETHINGPARTY_API USomethingPartyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Info Structs")
	FCharacterInfoStruct ChosenClass;
};
