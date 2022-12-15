// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowSelectActor.h"
#include <Runtime/Engine/Classes/Components/PostProcessComponent.h>
#include <SplitTileActor.h>
#include <SomethingParty/SomethingPartyPlayerController.h>
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

// Sets default values
AArrowSelectActor::AArrowSelectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	//Crates a Static Mesh Component
	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Dice Mesh"));
	SetRootComponent(ArrowMesh);


	ArrowMesh->OnBeginCursorOver.AddDynamic(this, &AArrowSelectActor::EnableOutline);
	ArrowMesh->OnEndCursorOver.AddDynamic(this, &AArrowSelectActor::DisableOutline);
	ArrowMesh->OnClicked.AddDynamic(this, &AArrowSelectActor::SelectArrow);

	
}

// Called when the game starts or when spawned
void AArrowSelectActor::BeginPlay()
{
	Super::BeginPlay();
	
	ArrowDynamicMaterial = UMaterialInstanceDynamic::Create(ArrowMesh->GetMaterial(0), this);
	ArrowMesh->SetMaterial(0, ArrowDynamicMaterial);
}

void AArrowSelectActor::EnableOutline(UPrimitiveComponent* TouchedComponent)
{
	ASplitTileActor* SplitTile = GetOwner<ASplitTileActor>();
	ASomethingPartyPlayerController* Controller = GetWorld()->GetFirstPlayerController<ASomethingPartyPlayerController>();
	if (SplitTile->GetCharacterOnTile() && Controller == SplitTile->GetCharacterOnTile()->GetController()) {
		ArrowDynamicMaterial->SetScalarParameterValue(TEXT("EmissiveMultiplier"), 20);
		ArrowDynamicMaterial->SetScalarParameterValue(TEXT("HighlightMultiplier"), 2);
		//ArrowDynamicMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor(100, 150, 200));
	}
}

void AArrowSelectActor::DisableOutline(UPrimitiveComponent* TouchedComponent)
{
	ASplitTileActor* SplitTile = GetOwner<ASplitTileActor>();
	ASomethingPartyPlayerController* Controller = GetWorld()->GetFirstPlayerController<ASomethingPartyPlayerController>();
	if (SplitTile->GetCharacterOnTile() && Controller == SplitTile->GetCharacterOnTile()->GetController()) {
		ArrowDynamicMaterial->SetScalarParameterValue(TEXT("EmissiveMultiplier"), 0);
		ArrowDynamicMaterial->SetScalarParameterValue(TEXT("HighlightMultiplier"), 1);
	}
}

void AArrowSelectActor::SelectArrow(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	
	ASplitTileActor* SplitTile = GetOwner<ASplitTileActor>();
	ASomethingPartyPlayerController* Controller = GetWorld()->GetFirstPlayerController<ASomethingPartyPlayerController>();
	if (SplitTile->GetCharacterOnTile() && Controller == SplitTile->GetCharacterOnTile()->GetController()) {
		if (!HasAuthority()) {
			Controller->SelectTilePath(SplitTile, PathIndex);
		}
	}
}

// Called every frame
void AArrowSelectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UStaticMeshComponent* AArrowSelectActor::GetMesh()
{
	return ArrowMesh;
}

void AArrowSelectActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AArrowSelectActor, PathIndex);
}
