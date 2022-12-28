// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxHealth = 100.0f;
}

void UHealthComponent::HandleTakeDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f)
	{
		//Damage amount was 0 or less.
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	//OnHealthChanged.Broadcast(Health);
	if (Health <= 0) {
		OnDeath();
	}
}

void UHealthComponent::AddHealth(int amount)
{
	Health += amount;
	if (Health > MaxHealth)
		Health = MaxHealth;
	//OnHealthChanged.Broadcast(Health);
}

void UHealthComponent::OnDeath()
{
	GetOwner()->Destroy();
}

int UHealthComponent::GetHealth()
{
	return Health;
}

int UHealthComponent::GetMaxHealth()
{
	return MaxHealth;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	//Get the Owner of this Actor Component.
	AActor* MyOwner = GetOwner();

	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeDamage);

	}

	Health = MaxHealth;
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Health == 0) {
		OnDeath();
	}
}
