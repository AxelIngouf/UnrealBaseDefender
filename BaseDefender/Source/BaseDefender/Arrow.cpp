// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"

#include "Ennemy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AArrow::AArrow()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(30);
	CollisionComponent->AddRelativeLocation(FVector(30, 0, 0));
	RootComponent = CollisionComponent;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->SetUpdatedComponent(CollisionComponent);
	MovementComponent->MaxSpeed = 8000.0f;
	MovementComponent->InitialSpeed = 2500.0f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bShouldBounce = false;
	MovementComponent->Bounciness = 0.3f;

	Damage = 34;
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
    Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AArrow::BeginOverlap);
}

void AArrow::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Cast<AEnnemy>(OtherActor))
	{
		OtherActor->TakeDamage(Damage, FDamageEvent(UDamageType::StaticClass()), DamageInstigator, this);
		OnDestroy.Broadcast();
		Damage = 0;
	}
}

float AArrow::GetDamage() const
{
	return Damage;
}

void AArrow::SetDamage(float NewDamage)
{
	Damage = NewDamage;
}

void AArrow::AutoDestruct()
{
	if (CollisionComponent)
		CollisionComponent->DestroyComponent();
	if (MovementComponent)
		MovementComponent->DestroyComponent();
	Destroy();
}
