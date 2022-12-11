// Copyright Epic Games, Inc. All Rights Reserved.

#include "SomethingPartyCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SplineComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include <SomethingParty/SomethingPartyGameMode.h>
#include <SomethingParty/Public/TriggerableTileInterface.h>
#include <Dice.h>
#include <SomethingPartyGameState.h>
#include <SomethingPartyPlayerState.h>
#include "GameFramework/PlayerState.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <SplitTileActor.h>
#include <Components/WidgetComponent.h>

ASomethingPartyCharacter::ASomethingPartyCharacter()
{
	//Set up Replication
	bReplicates = true;
	SetReplicateMovement(true);

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	DiceNumberWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DiceNumber"));
	DiceNumberWidget->SetupAttachment(RootComponent);

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ASomethingPartyCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
	
	if (MovementTimeline.IsPlaying()) {
		MovementTimeline.TickTimeline(DeltaSeconds);
	}
}

bool ASomethingPartyCharacter::isMoving()
{
	return moving;
}

float ASomethingPartyCharacter::getTileWalkSpeed()
{
	return TileWalkSpeed;
}



void ASomethingPartyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Bind timeline functions
	FOnTimelineFloat ProgressTimeline;
	ProgressTimeline.BindUFunction(this, TEXT("MoveAlongSpline"));
	MovementTimeline.AddInterpFloat(MovementCurve, ProgressTimeline);
	FOnTimelineEvent OnTimelineFinished;
	OnTimelineFinished.BindUFunction(this, TEXT("OnEndReached"));
	MovementTimeline.SetTimelineFinishedFunc(OnTimelineFinished);


	//Get starting tile
	ATileActor* newTile = Cast<ASomethingPartyGameState>(GetWorld()->GetGameState())->StartTile;
	if (!newTile) {
		UE_LOG(LogTemp, Fatal, TEXT("Missing start tile..."));
	}
	CurrentTile = newTile;

	if (!MovementCurve) {
		UE_LOG(LogTemp, Fatal, TEXT("Missing movement curve..."));
	}

	//Get spline
	TArray<AActor*> ActorsToFind;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), FName("FindSpline"), ActorsToFind);
	if (ActorsToFind.Num() != 1) {
		UE_LOG(LogTemp, Fatal, TEXT("Only 1 spline actor in world allowed..."));
	}
	AActor* SplineActor = Cast<AActor>(ActorsToFind[0]);
	if (!SplineActor) {
		UE_LOG(LogTemp, Fatal, TEXT("No spline actor found..."));
	}
	if (SplineActor->GetComponentsByClass(USplineComponent::StaticClass()).Num() != 1) {
		UE_LOG(LogTemp, Fatal, TEXT("No spline component found in spline actor..."));
	}
	USplineComponent* SplineComponent = Cast<USplineComponent>(SplineActor->GetComponentsByClass(USplineComponent::StaticClass())[0]);
	if (!SplineComponent) {
		UE_LOG(LogTemp, Fatal, TEXT("No spline component found in spline actor..."));
	}
	MovementSpline = SplineComponent;


}
void ASomethingPartyCharacter::CreateMoveSpline(ATileActor* SplineStartTile, int amount)
{
	MovementSpline->ClearSplinePoints();
	MovementSpline->bDrawDebug = true;
	ATileActor* nextTilePoint = SplineStartTile;
	MovementSpline->AddSplinePoint(nextTilePoint->GetActorLocation(), ESplineCoordinateSpace::World, false); //Initial spline point
	if (nextTilePoint->getNextTile() != NULL) { //More than 1 tile
		nextTilePoint = nextTilePoint->getNextTile();
		for (int i = 0; i < amount; i++) {
			MovementSpline->AddSplinePoint(nextTilePoint->GetActorLocation(), ESplineCoordinateSpace::World, false); //Add spline point for each tile
			CurrentTile = nextTilePoint;
			if (Cast<ASplitTileActor>(CurrentTile)) {
				Cast<ASplitTileActor>(CurrentTile)->SetRemainingTiles(amount - i);
				break;
			}
			if (nextTilePoint->getNextTile() == NULL) { //If there is no tile after, end spline
				break;
			}
			
			nextTilePoint = nextTilePoint->getNextTile(); //Set nextTilePoint to next tile
		}
	}

	MovementSpline->UpdateSpline();
}
//Move character a given amount of tiles
void ASomethingPartyCharacter::Move()
{	
	

	moving = true;


	//Set proper play rate so it remains same speed no matter the length
	float TimelineLength = MovementSpline->GetSplineLength() / (TileWalkSpeed);
	MovementTimeline.SetPlayRate(1.f / TimelineLength);
	MovementTimeline.PlayFromStart();

}

UWidgetComponent* ASomethingPartyCharacter::GetDiceNumberWidget()
{
	return DiceNumberWidget;
}



//Called every tick while timeline is playing
void ASomethingPartyCharacter::MoveAlongSpline(float Value)
{
	const float SplineLength = MovementSpline->GetSplineLength();

	FVector CurrentSplineLocation = MovementSpline->GetLocationAtDistanceAlongSpline(Value * SplineLength, ESplineCoordinateSpace::World);
	FRotator CurrentSplineRotation = MovementSpline->GetRotationAtDistanceAlongSpline(Value * SplineLength, ESplineCoordinateSpace::World);
	
	
	SetActorLocationAndRotation(CurrentSplineLocation, CurrentSplineRotation);
}

//Called when timeline ended
void ASomethingPartyCharacter::OnEndReached()
{
	moving = false;

	

	//If tile is triggerable, then trigger action
	if (CurrentTile->Implements<UTriggerableTileInterface>()) {
		ITriggerableTileInterface* TriggerableTile = Cast<ITriggerableTileInterface>(CurrentTile);
		TriggerableTile->TriggerAction(this);
	}
	else {
		ASomethingPartyGameState* gs = Cast<ASomethingPartyGameState>(GetWorld()->GetGameState());
		gs->NextTurn();
	}
}

void ASomethingPartyCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASomethingPartyCharacter, moving);
}
