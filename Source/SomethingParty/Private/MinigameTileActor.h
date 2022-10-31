// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileActor.h"
#include "TriggerableTileInterface.h"
#include "MinigameTileActor.generated.h"


UCLASS()
class AMinigameTileActor : public ATileActor, public ITriggerableTileInterface
{
	GENERATED_BODY()
	
public:
	virtual void triggerAction() override;
};
