// Fill out your copyright notice in the Description page of Project Settings.


#include "LavaActor.h"
#include "HealthComponent.h"
#include <FloorIsLavaGameState.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values
ALavaActor::ALavaActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LavaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lava Mesh"));
	LavaMesh->SetWorldScale3D(FVector(100, 100, 10));
	LavaMesh->SetCollisionProfileName(TEXT("Trigger"));
	SetRootComponent(LavaMesh);

	


}

// Called when the game starts or when spawned
void ALavaActor::BeginPlay()
{
	Super::BeginPlay();
	LavaMesh->OnComponentBeginOverlap.AddDynamic(this, &ALavaActor::OnOverlapBegin);
	LavaMesh->OnComponentEndOverlap.AddDynamic(this, &ALavaActor::OnOverlapEnd);
	GetWorldTimerManager().SetTimer(BurnTimeHandle, this, &ALavaActor::Burn, 2, true);

	GameState = Cast<AFloorIsLavaGameState>(GetWorld()->GetGameState());
}

void ALavaActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(BurnTimeHandle);
}

void ALavaActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherComp) {
		UHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UHealthComponent>();
		if (HealthComponent) {
			ActorsInLava.Add(OtherActor);
		}
	}
}

void ALavaActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && OtherComp) {
		if (ActorsInLava.Contains(OtherActor)) {
			ActorsInLava.Remove(OtherActor);
		}
	}
}

void ALavaActor::Burn()
{
	
	if (!ActorsInLava.IsEmpty()) {
		for (AActor* Actor : ActorsInLava) {
			UGameplayStatics::ApplyDamage(Actor, 10.f, GetInstigatorController(), this, GenericDamageType);
		}
	}
}

// Called every frame
void ALavaActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (GameState) {
		if (GameState->MatchStarted) {
			AddActorLocalOffset(FVector(0, 0, 0.5));
		}
	}
}

