// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DiceNumberWidget.generated.h"

/**
 * 
 */
UCLASS()
class UDiceNumberWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* DiceNumberText;
	
};
