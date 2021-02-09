// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealthPoints = 1000.f;
	HealthPoints = MaxHealthPoints;

	HealthPercent = 1.f;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	HealthPoints = MaxHealthPoints; // If you modify a value in BP, this ensure both HP and Max HP are affected.
}

float UHealthComponent::ApplyComponentDamage(float Damage)
{
	HealthPoints -= Damage;
	if(HealthPoints <= 0)
	{
		Damage += HealthPoints;
		HealthPercent = 0.f;
		HealthPoints = 0.f;
	}
	else
	{
		HealthPercent = HealthPoints / MaxHealthPoints;
	}
	return Damage;
}

void UHealthComponent::SetMaxHealthPoints(const float MaxHealth)
{
	MaxHealthPoints = MaxHealth;
	HealthPoints = MaxHealthPoints;
	HealthPercent = 1.f;
}

void UHealthComponent::AddHealthPoints(const float Health)
{
	HealthPoints += Health;
	if (HealthPoints > MaxHealthPoints)
	{
		HealthPoints = MaxHealthPoints;
	}
	HealthPercent = HealthPoints / MaxHealthPoints;
}
