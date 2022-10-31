// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TriggerableTileInterface.generated.h"

UINTERFACE(MinimalAPI)
class UTriggerableTileInterface : public UInterface
{
	GENERATED_BODY()
};

class ITriggerableTileInterface
{
	GENERATED_BODY()

public:
	virtual void triggerAction();
};
