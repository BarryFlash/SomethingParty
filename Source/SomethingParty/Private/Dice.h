// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dice.generated.h"

UCLASS()
class ADice : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADice();

	UPROPERTY(Replicated)
		int DiceNumber;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* DiceMesh;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
