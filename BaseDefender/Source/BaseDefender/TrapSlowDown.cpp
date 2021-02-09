// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapSlowDown.h"
#include "AntiTrapComponent.h"
#include "BaseDefenderCharacter.h"
#include "Ennemy.h"
#include "PlayerMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ATrapSlowDown::ATrapSlowDown() : ATrap()
{
	NumberActorOverlapped = 0;
	TrapName = FString("Slow Down");

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere")).Object);
	StaticMesh->SetCollisionProfileName(FName(TEXT("NoCollision")));
	RootComponent = StaticMesh;

	AntiTrapComponent = CreateDefaultSubobject<UAntiTrapComponent>("AntiTrapComponent");
	AntiTrapComponent->SetupAttachment(RootComponent);

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	Cast<USphereComponent>(CollisionComponent)->InitSphereRadius(150.0f);
	CollisionComponent->SetupAttachment(RootComponent);
}

void ATrapSlowDown::BeginPlay()
{
	Super::BeginPlay();

	AddActorLocalOffset({ 0, 0, 60 });
}

void ATrapSlowDown::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (AEnnemy* OtherEnemy = Cast<AEnnemy>(OtherActor))
	{
		if (OtherComp->GetName() == FString("CollisionCylinder"))
		{
			OtherEnemy->SetSlowDown(true);
			SpeedUpPlayer();
			NumberActorOverlapped++;

			if (NumberActorOverlapped == 1)
			{
				GetWorldTimerManager().SetTimer(TimerHandle_Damaged, this, &ATrapSlowDown::SlowedEnemy, TimerDamageWhenSlowingEnemies, true);
			}
		}
	}
}

void ATrapSlowDown::EndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)
{
	Super::EndOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor)
	{
		if (AEnnemy* OtherEnemy = Cast<AEnnemy>(OtherActor))
		{

			if (OtherComp->GetName() == FString("CollisionCylinder"))
			{
				OtherEnemy->SetSlowDown(false);
				SlowDownPlayer();
				NumberActorOverlapped--;

				if(NumberActorOverlapped <= 0)
				{
					GetWorldTimerManager().ClearTimer(TimerHandle_Damaged);
				}
			}
		}
	}
}

void ATrapSlowDown::SlowDownPlayer() const
{
	ABaseDefenderCharacter* Player = Cast<ABaseDefenderCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	Cast<UPlayerMovementComponent>(Player->GetMovementComponent())->DeleteSpeedBonus();
}

void ATrapSlowDown::SpeedUpPlayer() const
{
	ABaseDefenderCharacter* Player = Cast<ABaseDefenderCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	Cast<UPlayerMovementComponent>(Player->GetMovementComponent())->AddSpeedBonus();
}

float ATrapSlowDown::Damaged()
{
	for(int i = 0; i < NumberActorOverlapped; i++)
	    Super::Damaged();

	return 1.f;
}

void ATrapSlowDown::AutoDestruct()
{
	Super::AutoDestruct();

	for(int i = 0; i < NumberActorOverlapped; i++)
	{
		SlowDownPlayer();
	}

	TArray<AActor*> OverlappedActors;
	GetOverlappingActors(OverlappedActors);

	for(AActor* Actor : OverlappedActors)
	{
	    if(AEnnemy* Enemy = Cast<AEnnemy>(Actor))
	    {
			Enemy->SetSlowDown(false);
	    }
	}
}

void ATrapSlowDown::SlowedEnemy()
{
	this->Damaged();
}
