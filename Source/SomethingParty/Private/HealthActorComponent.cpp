// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthActorComponent.h"

// Sets default values for this component's properties
UHealthActorComponent::UHealthActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 100;
	// ...
}


// Called when the game starts
void UHealthActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	AActor* MyOwner = GetOwner();

	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthActorComponent::HandleTakeDamage);

	}

	Health = MaxHealth;
}


void UHealthActorComponent::HandleTakeDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f)
	{
		//Damage amount was 0 or less.
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
	if (Health <= 0) {
		OnDeath();
	}
}

// Called every frame
void UHealthActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthActorComponent::AddHealth(int amount)
{
	Health += amount;
	if (Health > MaxHealth)
		Health = MaxHealth;
	OnHealthChanged.Broadcast(Health);
}

void UHealthActorComponent::OnDeath()
{
	GetOwner()->Destroy();
}

int UHealthActorComponent::GetHealth()
{
	return Health;
}

int UHealthActorComponent::GetMaxHealth()
{
	return MaxHealth;
}

