// Fill out your copyright notice in the Description page of Project Settings.


#include "Fortress.h"
#include "AntiTrapComponent.h"
#include "Components/BoxComponent.h"
#include "BaseDefender\HealthComponent.h"
#include "BaseDefenderGameMode.h"

// Sets default values
AFortress::AFortress()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube")).Object);
	StaticMesh->SetCollisionProfileName(FName(TEXT("BlockAll")));
	RootComponent = StaticMesh;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitBoxExtent(FVector(100, 100, 100));
	CollisionComponent->SetupAttachment(RootComponent);

	AntiTrapComponent = CreateDefaultSubobject<UAntiTrapComponent>("AntiTrapComponent");
	AntiTrapComponent->SetupAttachment(CollisionComponent);

	Health = CreateDefaultSubobject<UHealthComponent>(FName(TEXT("HealthComponent")));
}

void AFortress::EventDamage(float Damage)
{
	if(Godmode)
		return;
	
	ABaseDefenderGameMode* GameMode = Cast<ABaseDefenderGameMode>(GetWorld()->GetAuthGameMode());

	if(GameMode)
	{
		GameMode->ApplyFortressDamage(Damage);

		Health->ApplyComponentDamage(Damage);

		if (Health->GetHealthPoints() <= 0)
		{
			Destroy();
		}
	}
}

UHealthComponent* AFortress::GetHealthComponent()
{
	return Health;
}

