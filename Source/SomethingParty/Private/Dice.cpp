// Fill out your copyright notice in the Description page of Project Settings.


#include "Dice.h"
#include <SomethingParty/SomethingPartyCharacter.h>

// Sets default values
ADice::ADice()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Crates a Static Mesh Component
	DiceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Dice Mesh"));
	DiceMesh->SetCollisionProfileName(TEXT("BlockAll"));
	SetRootComponent(DiceMesh);

}

// Called when the game starts or when spawned
void ADice::BeginPlay()
{
	Super::BeginPlay();

	//Call the OnHit Function
	DiceMesh->OnComponentHit.AddDynamic(this, &ADice::OnHit);
}


//Set DiceNumber to a random number 1-10
void ADice::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	int DiceNumber = FMath::RandRange(1, 10);

	ASomethingPartyCharacter* Character = Cast<ASomethingPartyCharacter>(OtherActor);

	
	if (Character) {
		if (!Character->isMoving()) {
			UE_LOG(LogTemp, Warning, TEXT("DICE NUMBER: %i"), DiceNumber);
			Character->Move(DiceNumber);
		}
	}
}

// Called every frame
void ADice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

