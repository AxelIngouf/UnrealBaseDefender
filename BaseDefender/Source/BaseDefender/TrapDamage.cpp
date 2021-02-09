// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapDamage.h"
#include "AntiTrapComponent.h"
#include "HealthComponent.h"
#include "Ennemy.h"

ATrapDamage::ATrapDamage() : ATrap()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere")).Object);
	StaticMesh->SetCollisionProfileName(FName(TEXT("NoCollision")));

	RootComponent = StaticMesh;

	AntiTrapComponent = CreateDefaultSubobject<UAntiTrapComponent>("AntiTrapComponent");
	AntiTrapComponent->SetupAttachment(RootComponent);
}

void ATrapDamage::EffectIntensity(const float IntensityRatio)
{
	DamagePerSecond *= IntensityRatio;
}

void ATrapDamage::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (Cast<AEnnemy>(OtherActor))
	{

		if (OtherComp->GetName() == FString("CollisionCylinder"))
		{
			CausePainTo(OtherActor);

			// Start timer if none is active
			if (!GetWorldTimerManager().IsTimerActive(TimerHandle_PainTimer))
			{
				GetWorldTimerManager().SetTimer(TimerHandle_PainTimer, this, &ATrapDamage::PainTimer, PainInterval, true);
			}
		}
	}
}

float ATrapDamage::CausePainTo(AActor* Actor)
{
	if (DamagePerSecond > 0.f && bActivated)
	{
		Actor->TakeDamage(DamagePerSecond * PainInterval, FDamageEvent(UDamageType::StaticClass()), DamageInstigator, this);
		return this->Damaged();
	}
	return Health->HealthPoints;
}

void ATrapDamage::PainTimer()
{
	TSet<UPrimitiveComponent*> TouchingComponents;
	GetOverlappingComponents(TouchingComponents);

	bool	bOverlapping = false;

	for (UPrimitiveComponent* const Component : TouchingComponents)
	{
		if (Component)
		{
			if (AEnnemy* Enemy = Cast<AEnnemy>(Component->GetAttachmentRootActor()))
			{
				if (Component->GetName() == FString("CollisionCylinder"))
				{
					bOverlapping = true;
					if(CausePainTo(Enemy) <= 0.f)
					{
						break;
					}
				}
			}
		}
	}

	// Stop timer if nothing is overlapping trap
	if (!bOverlapping)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_PainTimer);
	}
}
