// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapDamageLine.h"

#include "Components/CapsuleComponent.h"

ATrapDamageLine::ATrapDamageLine() : ATrapDamage()
{
	PainInterval = 10.f;
	DamagePerSecond = 30.f;
	TrapName = FString("Turret");

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DamageZone"));
	Cast<UCapsuleComponent>(CollisionComponent)->InitCapsuleSize(30, 260);
	CollisionComponent->SetupAttachment(RootComponent);

	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ATrapDamageLine::DeactivateTrap()
{
	Super::DeactivateTrap();

	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
