// Fill out your copyright notice in the Description page of Project Settings.


#include "Dice.h"
#include <SomethingParty/SomethingPartyCharacter.h>
#include "GameFramework/PlayerState.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <SomethingParty/SomethingPartyGameMode.h>


// Sets default values
ADice::ADice()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Crates a Static Mesh Component
	DiceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Dice Mesh"));
	DiceMesh->SetCollisionProfileName(TEXT("BlockAll"));
	SetRootComponent(DiceMesh);

	bReplicates = true;
}

// Called when the game starts or when spawned
void ADice::BeginPlay()
{
	Super::BeginPlay();

	//Call the OnHit Function
	DiceMesh->OnComponentHit.AddDynamic(this, &ADice::OnHit);
}

void ADice::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADice, DiceNumber);
}


//Set DiceNumber to a random number 1-10
void ADice::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ASomethingPartyCharacter* Character = Cast<ASomethingPartyCharacter>(OtherActor);
	if (Character) {
		if (HasAuthority())
			Cast<ASomethingPartyGameMode>(GetWorld()->GetAuthGameMode())->RollDice(Character, this);
		//Character->Move(DiceNumber);
		Destroy();
	}
	
	
}

// Called every frame
void ADice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

