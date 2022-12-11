// Fill out your copyright notice in the Description page of Project Settings.


#include "Dice.h"
#include <SomethingParty/SomethingPartyCharacter.h>
#include "GameFramework/PlayerState.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <SomethingParty/SomethingPartyGameMode.h>
#include <SomethingPartyPlayerState.h>
#include "Components/TextBlock.h"
#include <DiceNumberWidget.h>
#include <SomethingPartyPlayerState.h>


// Sets default values
ADice::ADice()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	//Crates a Static Mesh Component
	DiceMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Dice Mesh"));
	DiceMesh->SetCollisionProfileName(TEXT("BlockAll"));
	DiceMesh->SetupAttachment(RootComponent);

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
		ASomethingPartyPlayerState* playerState = Character->GetPlayerState<ASomethingPartyPlayerState>();
		UDiceNumberWidget* DiceNumWidget = Cast<UDiceNumberWidget>(Character->GetDiceNumberWidget()->GetWidget());
		if (DiceNumWidget->DiceNumberText) {
			DiceNumWidget->DiceNumberText->SetText(FText::FromString(FString::FromInt(DiceNumber)));
			DiceNumWidget->SetVisibility(ESlateVisibility::Visible);
		}
		if (playerState->WaitingToRoll) {
			playerState->WaitingToRoll = false;
		}
		
		Destroy();
	}
	
	
}

// Called every frame
void ADice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

