// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapDamageZone.h"
#include "Components/SphereComponent.h"
#include "HealthComponent.h"

ATrapDamageZone::ATrapDamageZone() : ATrapDamage()
{
	PainInterval = 0.1f;
	DamagePerSecond = 270.0f;
	Health->SetMaxHealthPoints(200.f);
	TrapName = FString("Damage Zone");

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	Cast<USphereComponent>(CollisionComponent)->InitSphereRadius(150.0f);
	CollisionComponent->SetupAttachment(RootComponent);
}
