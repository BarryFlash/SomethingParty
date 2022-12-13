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

	UPROPERTY(ReplicatedUsing=OnRep_DiceNumber)
		int DiceNumber;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	USkeletalMeshComponent* DiceMesh;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnRep_DiceNumber();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere, Category = "Dice")
	class UWidgetComponent* DiceNumberWidget;

	USkeletalMeshComponent* GetMeshComponent();

	void SetDiceNumber(int NewDiceNumber);
};
