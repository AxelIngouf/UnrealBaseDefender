// Fill out your copyright notice in the Description page of Project Settings.


#include "LifePotion.h"

#include "BaseDefender\HealthComponent.h"
#include "BaseDefenderCharacter.h"
#include "Components/SphereComponent.h"

#define MAXZ 15
#define DELTAMAX 0.03

// Sets default values
ALifePotion::ALifePotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	Cast<USphereComponent>(CollisionComponent)->InitSphereRadius(40.0f);
	RootComponent = CollisionComponent;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere")).Object);
	StaticMesh->SetWorldScale3D(FVector(0.2f));
	StaticMesh->SetCollisionProfileName(FName(TEXT("NoCollision")));
	StaticMesh->SetupAttachment(RootComponent);

	HealthBonusAmount = 10.f;
	bAscending = true;
}

// Called when the game starts or when spawned
void ALifePotion::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ALifePotion::BeginOverlap);
}

void ALifePotion::Tick(float DeltaSeconds)
{
	float ZMovement = 0.f;

	if(DeltaSave > DELTAMAX)
	{
		while (DeltaSave > DELTAMAX)
			DeltaSave -= DELTAMAX;

		if (bAscending)
		{
			if (StaticMesh->GetRelativeLocation().Z < MAXZ)
			{
				ZMovement = .2f;
			}
			else
			{
				bAscending = false;
			}
		}
		else
		{
			if (StaticMesh->GetRelativeLocation().Z > 0.f)
			{
				ZMovement = -.2f;
			}
			else
			{
				bAscending = true;
			}
		}

		StaticMesh->AddRelativeLocation(FVector(0.f, 0.f, ZMovement));

		StaticMesh->AddRelativeRotation(FRotator(0.f, 4.f, 0.f));
	    
	}
	DeltaSave += DeltaSeconds;

}

void ALifePotion::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(ABaseDefenderCharacter* Player = Cast<ABaseDefenderCharacter>(OtherActor))
	{
		if(Player->GetHealthComponent()->HealthPercent < 1.f)
		{
			Player->GetHealthComponent()->AddHealthPoints(HealthBonusAmount);
			OnPickedUp.Broadcast();
			Destroy();
		}
	}
}

