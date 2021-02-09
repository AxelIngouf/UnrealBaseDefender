// Fill out your copyright notice in the Description page of Project Settings.


#include "Trap.h"

#include "BaseDefenderGameMode.h"
#include "BaseDefenderCharacter.h"
#include "HealthComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATrap::ATrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetCanBeDamaged(false);

	Health = CreateDefaultSubobject<UHealthComponent>(FName("HealthComponent"));
	Health->SetMaxHealthPoints(100.f);

	DamageTakenOnUse = 5.f;
	bActivated = true;
}

// Called when the game starts or when spawned
void ATrap::BeginPlay()
{
	Super::BeginPlay();

	if(bActivated)
	{
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATrap::BeginOverlap);
		CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ATrap::EndOverlap);

		GetWorldTimerManager().SetTimer(TimerHandle_AutoDamage, this, &ATrap::ProgressiveAutoDestruction, TimerAutoDamage, true);
	}

}

void ATrap::AutoDestruct()
{
	if(IsValid(this))
	{
		if (bActivated)
		{
			ABaseDefenderCharacter* Player = Cast<ABaseDefenderCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
			if (Player)
			{
				Player->IncrementAvailableTrap(TrapIndexPlayerInventory, 1);
				ABaseDefenderGameMode* GameMode = Cast<ABaseDefenderGameMode>(UGameplayStatics::GetGameMode(this));
				if (GameMode)
				{
					GameMode->OnTrapDestroyed.Broadcast(GetActorLocation());
				}
			}
		}

		Destroy();
	}
}

void ATrap::ProgressiveAutoDestruction()
{
	Health->ApplyComponentDamage(DamageAutoDestruction);
	if (Health->HealthPoints <= 0.f)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_AutoDamage);
		AutoDestruct();
	}
}

void ATrap::DeactivateTrap()
{
	bActivated = false;
	SetActorEnableCollision(false);
}

void ATrap::ResizeTrap(const float ResizeRatio) const
{
	CollisionComponent->SetRelativeScale3D({ ResizeRatio, ResizeRatio, ResizeRatio });
}

void ATrap::SetTrapIndex(const int Index)
{
	TrapIndexPlayerInventory = Index;
}

float ATrap::Damaged()
{
	Health->ApplyComponentDamage(DamageTakenOnUse);
	if(Health->HealthPoints <= 0.f)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_AutoDamage);
		AutoDestruct();
		return 0.f;
	}
	return Health->HealthPoints;
}
