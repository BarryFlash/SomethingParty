// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArrowSelectActor.generated.h"

UCLASS()
class AArrowSelectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrowSelectActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Mesh", Replicated)
	UStaticMeshComponent* ArrowMesh;

	class UMaterialInstanceDynamic* ArrowDynamicMaterial;

	UFUNCTION()
	void EnableOutline(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void DisableOutline(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	void SelectArrow(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int PathIndex;

	UStaticMeshComponent* GetMesh();

};
